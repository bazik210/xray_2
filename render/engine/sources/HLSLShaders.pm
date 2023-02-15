
package Shaders::D3D10;

use Data::Dumper;

@ISA = qw/Shaders/;

our @PixelShaderProfiles = qw/ps_4_0/;
our @VertexShaderProfiles = qw/vs_4_0/;

sub new { bless {} }

sub PlatformNames { qw/D3D1x/ }

sub DefaultOutMode { 'b'; }
sub Caps { 'Ureg Bin BinSize Uname' }

sub GetSem
{
    my ($vt,$vn,$vs) = ($_[0]->{type},$_[0]->{name},$_[0]->{sem});
    
    # Uniforms shouldn't have semantics in D3D.
    if ($_[0]->{vary} eq 'uniform' )
    {
        return '';
    }
    
    if ($vs eq '') 
    {
        if ($vn =~ /^a?tc([0-1]*)/) { $vs = 'TEXCOORD'.(0+$1); }
        elsif ($vn =~ /color/)      { $vs = 'COLOR0'; }
        elsif ($vn =~ /factor/)     { $vs = 'COLOR1'; }
        elsif ($vn =~ /^v?pos/)     { $vs = 'SV_POSITION'; }
        elsif ($vn =~ /batch/)      { $vs = 'COLOR1'; }
        elsif ($vn =~ /fucxadd/)    { $vs = 'TEXCOORD3'; }
        elsif ($vn =~ /fucxmul/)    { $vs = 'TEXCOORD4'; }
    }
    elsif (!$vs =~ /^SV_/) { $vs = uc($vs); }
  return $vs;
}

sub Param
{
    $_ = $_[1];
    
  if ($_->{type} =~ /^sampler(.*)$/) {
    my $s = "Texture$1 $_->{name} : register(t\$texn);\nsampler   $_->{name}_s : register(s\$texn)".($_[2]?';':'');
        $_->{uofs} = $texn;
        return $s, 2, {qw/texn $$texn++/};
    }

    my ($vt,$vn,$vs) = ($_->{type},$_->{name},GetSem($_));
    $vt =~ s/vec/float/g;
  if (exists $_->{dim}) {
        $vn .= "[$_->{dim}]";
    }

  if ($_[0]->{stage} eq 'v' && $_->{vary} eq 'varying') { $vt = "out $vt"; }
  elsif ($_[0]->{stage} eq 'f' && $_->{vary} eq 'fragout') { $vt = "out $vt"; }
    my $s = "$vt $vn";
    $s.= " : $vs" if $vs;

  $s .= (($_->{vary} eq 'uniform') ? ';' : ',') if $_[2];
  
  Shaders::ParamD3D($s);
  return $s, (($_->{vary} eq 'uniform') ? 1 : 0);
}

sub ParamSort
{
  my $c = $Shaders::VarySort{$_[1]->{vary}} <=> $Shaders::VarySort{$_[2]->{vary}};
  return $c if ($c);
  my $as = GetSem($_[1]).$_[1]->{name}; $as =~ s/^SV_/zzzSV_/;
  my $bs = GetSem($_[2]).$_[2]->{name}; $bs =~ s/^SV_/zzzSV_/;
  return $as cmp $bs;
}

sub ParamCategory
{
  if ($_->{type} =~ /^sampler(.*)$/) {
    return 2;
                            }
  elsif ($_[1]->{vary} eq 'uniform') {
    return 1;
                        }
  else {
    return 0;
    }
}

sub BuiltinParams
{
  return ($Shaders::stage eq 'f' ? ({vary => 'fragout', type => 'float4', name => 'fcolor', sem => 'SV_Target'}) : ());
}
    
sub BeginUniforms { ("cbuffer ".($_[0]->{stage} eq 'f' ? 'P' : 'V')."SConstants {\n",'    '); }
sub EndUniforms { "}\n" }

sub Block
{
  my ($this, $stage, $argsr, $b) = @_;
  my %args = %{$argsr};

  #$b = Shaders::BlockD3D($this, $stage, $argsr, $b);
  Shaders::ParamD3D($b);
  $b =~ s/(^|\W)mix(\W)/\1lerp\2/g;
  $b =~ s/(^|\W)texture([1-3]D\W)/\1tex\2/g;
  $b =~ s/(^|\W)(?:float|half|int)[1-4] *\(([^,()]*)\)/\1\2/g;

  $b =~ s/tex(?:ture)?2D *\((\w+) *,(.*)\)/$1.Sample(\1_s, $2)/g;
  $b =~ s/tex(?:ture)?2DLod *\((\w+) *,(.*)\)/$1.SampleLevel(\1_s, $2)/g;

  $b =~ s/float([234]) *\($Shaders::subexpr\)/"float$1(".join(',',($2)x$1).")"/ge;
  
  return $b;
}

sub FileExt
{
  return 'hlsl';
}

sub GetOutputs
{
    my $this = $_[0];
    print "$_[1]\n";
    if ($_[1] eq 'b') 
    {
        return [0, 

                sub {   return if Shaders::CheckOut(@_);
                        my $f = "$this->{dir}/$_[0].".$this->FileExt($stage);
                        open(OUT,">$f") || die $f;
                        print OUT $_[1];
                        close(OUT);
                        
                        my @profileList = ($_[0] =~ /^V/) ? @VertexShaderProfiles : @PixelShaderProfiles;
                        my $shaderCompiled = 0;
                        foreach my $prof (@profileList)
                        {
                            Shaders::OutputMessage( 2, "fxc.exe /nologo /E main /T $prof /Fh $f.h /Vn pBinary_$_[0] $f\n" );
                            my $resultCode = system("fxc.exe /nologo /E main /T $prof /Fh $f.h /Vn pBinary_$_[0] $f");
                            if (!$resultCode)
                            {
                                my $outFile = "$this->{dir}/$this->{name}_BinaryShaders.cpp";
                                system("cat $f.h >> $outFile; rm -f $f.h");
                                Shaders::OutputMessage( 3, "---------------------------\n" . `cat $f` . "\n---------------------------\n");
                                system("echo -e \"const unsigned pBinary_${_[0]}_size = sizeof(pBinary_${_[0]});\\r\\n\" >> $outFile");
                                $shaderCompiled = 1;
                                last;
                            }
                        }
                        $shaderCompiled or die "$f: compilation failed\n\nShader Text:\n" . `cat $f`;
                    },
                       
                sub {   
                        my $filename = "$this->{dir}/$this->{name}_BinaryShaders.cpp $this->{dir}/*.".$this->FileExt($stage);
                        `rm -f $filename`;
                        Shaders::OutputMessage(2, "Deleting $filename\n");
                        $this->OpenFiles('f');
                        my $dxdir = `cygpath -ua '$ENV{'DXSDK_DIR'}'`;
                        chomp($dxdir);
                        $ENV{'PATH'} .= ":$dxdir/Utilities/bin/x86";
                        `echo -e "namespace Scaleform { namespace Render { namespace D3D1x { \\r\\n" >> $this->{dir}/$this->{name}_BinaryShaders.cpp`;
                    },
                    
                sub { 
                        $this->WriteHeaders(); 
                        $this->CloseFiles('f');
                        `echo -e "}}}\\r\\n" >> $this->{dir}/$this->{name}_BinaryShaders.cpp`;
                    }
               ];
    }
  return $_[0]->Shaders::GetOutputs($_[1]);
}

sub GetAttrStruct {
    my ($Platform, $name, $stage) = @_;
    if ($stage eq 'v')
    {
        return "    struct ${name}AttrDesc\n".
               "    {\n".
               "        const char*   Name;\n".
               "        unsigned      Attr;\n".
               "        const char*   SemanticName;\n".
               "        unsigned      SemanticIndex;\n".
               "        unsigned      Format;      /* DXGI_FORMAT */\n".
               "    };\n";
    }
    return 0;
}

sub GetVAttrDesc {
    my ($Platform, $vt,$vn,$vs) = ($_[0], $_[1]->{type},$_[1]->{name},$_[1]->{sem});
    my $base = "\"$vn\", ";

    $vt =~ /([0-9])/;
    my $typeComponents = $1 ? $1 : 1;
    $base .= "$typeComponents";
    my $vertexType;
    if ($vn =~ /^a?tc([0-9]*)/) 
    { 
        $base .= ' | VET_TexCoord'; 
        $vertexType = "DXGI_FORMAT_R32G32_FLOAT"; 
    }
    elsif ($vn =~ /^a?color/)   
    { 
        $base .= ' | VET_Color';    
        $vertexType = "DXGI_FORMAT_R8G8B8A8_UINT"; 
    }
    elsif ($vn =~ /^a?factor/)  
    { 
        $base .= ' | VET_Color | (1 << VET_Index_Shift)'; 
        $vertexType = "DXGI_FORMAT_R8G8B8A8_UINT"; 
    }
    elsif ($vn =~ /^a?pos/)     
    { 
        $base .= ' | VET_Pos'; 
        $vertexType = "DXGI_FORMAT_R32G32_FLOAT"; 
        # TODO: SInt16 formats?.
    }
    elsif ($vn =~ /^v?batch$/)    
    { 
        $base .= ' | VET_Instance8'; 
        $vertexType = "DXGI_FORMAT_R8_UINT"; 
    }

    # Store D3DDECL specific elements directly in the attributes. This way, we don't
    # need to guess at runtime what the components should be.
    
    $base .= ', ';
    
    # Semantic, SemanticIndex, Format
    $vs = GetSem($_[1]);
    $vs =~ s/([0-9])//;
    $vsindex = $1 ? $1 : '0';
    $base .= "\"$vs\", $vsindex, $vertexType";  
    
    return $base;
}

sub WriteHeaders
{
	my %stages = qw/v Vertex f Frag/;
	foreach my $stage ('v','f') {
		foreach my $g (keys %Shaders::groups) 
        {
			my $Group = $Shaders::groups{$g};
			my @out = @{$Group->{$stage.'out'}};
			foreach (@out) 
            {
				if (ref($_)) 
                {
					my $fn = Shaders::GetFName($stage, @{$_->{blocks}});
					my $sym = "pBinary_${fn}";
					print Shaders::OUTC "extern const BYTE $sym\[\];\n";
					print Shaders::OUTC "extern const unsigned ${sym}_size;\n";
				}
			}
		}
	}

        Shaders::WriteHeaders();
}

sub BinaryShader { return "pBinary_$_[2]" };

# just cast it in C.
sub ShaderBinaryType { 'const void*' } 

sub GetBatchVar 
{
    my $v = $_[1], $array = 'f';
    my $size = $v->{dim} ? $v->{dim} : 1;
  
    # In D3D, always make everything a float4 - if it comes in as a matrix
    if ($v->{type} =~ /(?:^mat)|([0-9])x([0-9])/) 
    {
        if ( $1 eq 'mat' ) 
        {
            $v->{type} = "float4x4";
        }
    }
    return ($array, $size);
}

sub GetBatchIndexVariables
{
    my ($Platform, $argsRef) = @_;
    my %args = %$argsRef;
    my %batchIndexers;
    if ( $Platform->Caps() =~ /Batchfv/ )
    {
        my @factorArgs = (grep $_->{'name'} =~/a?factor/ && $_->{'vary'} ne 'varying', values %args);
        if ( $#factorArgs >= 0 )
        {
            $batchIndexers{'v'} = {qw/vary subattr type float name/, $factorArgs[0]{'name'}. ".b"};
        }
        else
        {
            $batchIndexers{'v'} = {qw/vary attribute type float name vbatch/};
        }
    }
    else
    {
        my @factorArgs = (grep $_->{'name'} =~/a?factor/ && $_->{'vary'} eq 'varying', values %args);
        if ( $#factorArgs >= 0 )
        {
            $batchIndexers{'f'} = {qw/vary subattr type float name/, $factorArgs[0]{'name'}. ".b"};
            $batchIndexers{'v'} = {qw/vary subattr type float name/, 'a'.$factorArgs[0]{'name'}. ".b"};
        }
        else
        {
            $batchIndexers{'f'} = {qw/vary varying type float name batch/};
            $batchIndexers{'v'} = {qw/vary attribute type float name vbatch/};
        }
    }
    return \%batchIndexers;
}

new();
