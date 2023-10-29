#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace xray {
namespace model_editor {
	class bsp_tree;
	/// <summary>
	/// Summary for bsp_tree_panel
	/// </summary>
	public ref class bsp_tree_panel  : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		bsp_tree_panel( bsp_tree* tree ):
		m_bsp_tree(tree)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void set_model_name( System::String^ model_name );
		property System::Windows::Forms::TreeView^ tree_view
		{
			System::Windows::Forms::TreeView^ get()
			{
				return bsp_tree_view;
			}
		}
		void on_restore_file_ready	( resources::queries_result& data );

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~bsp_tree_panel()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TreeView^  bsp_tree_view;
	private: System::Windows::Forms::Panel^  panel1;

	private: System::Windows::Forms::TrackBar^  min_square_trackbar;

	private: System::Windows::Forms::Label^  lbl_min_square;






	private: System::Windows::Forms::CheckBox^  chb_aabb;
	private: System::Windows::Forms::CheckBox^  chb_solid;




	private: System::Windows::Forms::ContextMenuStrip^  bsp_tree_context_menu;

	private: System::Windows::Forms::ToolStripMenuItem^  serializeNodeToolStripMenuItem;
	private: System::Windows::Forms::SaveFileDialog^  bsp_node_save_dialog;
	private: System::Windows::Forms::ToolStripMenuItem^  serialiseWholeTreeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadWholeTreeToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  bsp_node_open_dialog;
	private: System::Windows::Forms::CheckBox^  chb_draw_divider;
	private: System::Windows::Forms::CheckBox^  chb_double_sided;
	private: System::Windows::Forms::CheckBox^  chb_splitted_blanks;
	private: System::Windows::Forms::CheckBox^  chb_portal_blanks;
	private: System::Windows::Forms::CheckBox^  chb_splitted_geometry;
	private: System::Windows::Forms::CheckBox^  chb_geometry;
	private: System::Windows::Forms::CheckBox^  chb_portals;
	private: System::Windows::Forms::TrackBar^  slimness_threshold_trackbar;
	private: System::Windows::Forms::Label^  lbl_slimness_threshold;
	private: System::Windows::Forms::CheckBox^  chb_outer_edges;
	private: System::Windows::Forms::Label^  lbl_position_difference;

	private: System::Windows::Forms::Label^  lbl_square_difference;

	private: System::Windows::Forms::TrackBar^  position_difference_trackbar;
	private: System::Windows::Forms::TrackBar^  square_difference_trackbar;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;

	private: System::ComponentModel::IContainer^  components;


	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->bsp_tree_view = (gcnew System::Windows::Forms::TreeView());
			this->bsp_tree_context_menu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->serializeNodeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->serialiseWholeTreeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadWholeTreeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->lbl_position_difference = (gcnew System::Windows::Forms::Label());
			this->lbl_square_difference = (gcnew System::Windows::Forms::Label());
			this->position_difference_trackbar = (gcnew System::Windows::Forms::TrackBar());
			this->square_difference_trackbar = (gcnew System::Windows::Forms::TrackBar());
			this->chb_outer_edges = (gcnew System::Windows::Forms::CheckBox());
			this->lbl_slimness_threshold = (gcnew System::Windows::Forms::Label());
			this->slimness_threshold_trackbar = (gcnew System::Windows::Forms::TrackBar());
			this->chb_portals = (gcnew System::Windows::Forms::CheckBox());
			this->chb_splitted_blanks = (gcnew System::Windows::Forms::CheckBox());
			this->chb_portal_blanks = (gcnew System::Windows::Forms::CheckBox());
			this->chb_splitted_geometry = (gcnew System::Windows::Forms::CheckBox());
			this->chb_geometry = (gcnew System::Windows::Forms::CheckBox());
			this->chb_draw_divider = (gcnew System::Windows::Forms::CheckBox());
			this->chb_double_sided = (gcnew System::Windows::Forms::CheckBox());
			this->chb_solid = (gcnew System::Windows::Forms::CheckBox());
			this->chb_aabb = (gcnew System::Windows::Forms::CheckBox());
			this->lbl_min_square = (gcnew System::Windows::Forms::Label());
			this->min_square_trackbar = (gcnew System::Windows::Forms::TrackBar());
			this->bsp_node_save_dialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->bsp_node_open_dialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->bsp_tree_context_menu->SuspendLayout();
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->position_difference_trackbar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->square_difference_trackbar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->slimness_threshold_trackbar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->min_square_trackbar))->BeginInit();
			this->SuspendLayout();
			// 
			// bsp_tree_view
			// 
			this->bsp_tree_view->ContextMenuStrip = this->bsp_tree_context_menu;
			this->bsp_tree_view->Dock = System::Windows::Forms::DockStyle::Fill;
			this->bsp_tree_view->Location = System::Drawing::Point(0, 0);
			this->bsp_tree_view->Name = L"bsp_tree_view";
			this->bsp_tree_view->Size = System::Drawing::Size(292, 390);
			this->bsp_tree_view->TabIndex = 0;
			this->bsp_tree_view->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &bsp_tree_panel::bsp_tree_view_AfterSelect);
			// 
			// bsp_tree_context_menu
			// 
			this->bsp_tree_context_menu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->serializeNodeToolStripMenuItem, 
				this->serialiseWholeTreeToolStripMenuItem, this->toolStripMenuItem1, this->loadWholeTreeToolStripMenuItem});
			this->bsp_tree_context_menu->Name = L"bsp_tree_context_menu";
			this->bsp_tree_context_menu->Size = System::Drawing::Size(168, 98);
			// 
			// serializeNodeToolStripMenuItem
			// 
			this->serializeNodeToolStripMenuItem->Name = L"serializeNodeToolStripMenuItem";
			this->serializeNodeToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->serializeNodeToolStripMenuItem->Text = L"Serialise node";
			this->serializeNodeToolStripMenuItem->Click += gcnew System::EventHandler(this, &bsp_tree_panel::serializeNodeToolStripMenuItem_Click);
			// 
			// serialiseWholeTreeToolStripMenuItem
			// 
			this->serialiseWholeTreeToolStripMenuItem->Name = L"serialiseWholeTreeToolStripMenuItem";
			this->serialiseWholeTreeToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->serialiseWholeTreeToolStripMenuItem->Text = L"Serialise whole tree";
			this->serialiseWholeTreeToolStripMenuItem->Click += gcnew System::EventHandler(this, &bsp_tree_panel::serialiseWholeTreeToolStripMenuItem_Click);
			// 
			// loadWholeTreeToolStripMenuItem
			// 
			this->loadWholeTreeToolStripMenuItem->Name = L"loadWholeTreeToolStripMenuItem";
			this->loadWholeTreeToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->loadWholeTreeToolStripMenuItem->Text = L"Load whole tree";
			this->loadWholeTreeToolStripMenuItem->Click += gcnew System::EventHandler(this, &bsp_tree_panel::loadWholeTreeToolStripMenuItem_Click);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->lbl_position_difference);
			this->panel1->Controls->Add(this->lbl_square_difference);
			this->panel1->Controls->Add(this->position_difference_trackbar);
			this->panel1->Controls->Add(this->square_difference_trackbar);
			this->panel1->Controls->Add(this->chb_outer_edges);
			this->panel1->Controls->Add(this->lbl_slimness_threshold);
			this->panel1->Controls->Add(this->slimness_threshold_trackbar);
			this->panel1->Controls->Add(this->chb_portals);
			this->panel1->Controls->Add(this->chb_splitted_blanks);
			this->panel1->Controls->Add(this->chb_portal_blanks);
			this->panel1->Controls->Add(this->chb_splitted_geometry);
			this->panel1->Controls->Add(this->chb_geometry);
			this->panel1->Controls->Add(this->chb_draw_divider);
			this->panel1->Controls->Add(this->chb_double_sided);
			this->panel1->Controls->Add(this->chb_solid);
			this->panel1->Controls->Add(this->chb_aabb);
			this->panel1->Controls->Add(this->lbl_min_square);
			this->panel1->Controls->Add(this->min_square_trackbar);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->panel1->Location = System::Drawing::Point(0, 390);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(292, 357);
			this->panel1->TabIndex = 1;
			// 
			// lbl_position_difference
			// 
			this->lbl_position_difference->AutoSize = true;
			this->lbl_position_difference->Location = System::Drawing::Point(199, 303);
			this->lbl_position_difference->Name = L"lbl_position_difference";
			this->lbl_position_difference->Size = System::Drawing::Size(46, 13);
			this->lbl_position_difference->TabIndex = 20;
			this->lbl_position_difference->Text = L"0.00001";
			// 
			// lbl_square_difference
			// 
			this->lbl_square_difference->AutoSize = true;
			this->lbl_square_difference->Location = System::Drawing::Point(202, 265);
			this->lbl_square_difference->Name = L"lbl_square_difference";
			this->lbl_square_difference->Size = System::Drawing::Size(28, 13);
			this->lbl_square_difference->TabIndex = 19;
			this->lbl_square_difference->Text = L"0.01";
			// 
			// position_difference_trackbar
			// 
			this->position_difference_trackbar->Location = System::Drawing::Point(10, 303);
			this->position_difference_trackbar->Maximum = 5000;
			this->position_difference_trackbar->Name = L"position_difference_trackbar";
			this->position_difference_trackbar->Size = System::Drawing::Size(183, 42);
			this->position_difference_trackbar->TabIndex = 18;
			this->position_difference_trackbar->TickFrequency = 200;
			this->position_difference_trackbar->Value = 1;
			this->position_difference_trackbar->Scroll += gcnew System::EventHandler(this, &bsp_tree_panel::position_difference_trackbar_Scroll);
			// 
			// square_difference_trackbar
			// 
			this->square_difference_trackbar->Location = System::Drawing::Point(10, 254);
			this->square_difference_trackbar->Name = L"square_difference_trackbar";
			this->square_difference_trackbar->Size = System::Drawing::Size(186, 42);
			this->square_difference_trackbar->TabIndex = 17;
			this->square_difference_trackbar->Value = 1;
			this->square_difference_trackbar->Scroll += gcnew System::EventHandler(this, &bsp_tree_panel::square_difference_trackbar_Scroll);
			// 
			// chb_outer_edges
			// 
			this->chb_outer_edges->AutoSize = true;
			this->chb_outer_edges->Location = System::Drawing::Point(150, 202);
			this->chb_outer_edges->Name = L"chb_outer_edges";
			this->chb_outer_edges->Size = System::Drawing::Size(87, 17);
			this->chb_outer_edges->TabIndex = 16;
			this->chb_outer_edges->Text = L"Draw circuits";
			this->chb_outer_edges->UseVisualStyleBackColor = true;
			this->chb_outer_edges->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_outer_edges_CheckedChanged);
			// 
			// lbl_slimness_threshold
			// 
			this->lbl_slimness_threshold->AutoSize = true;
			this->lbl_slimness_threshold->Location = System::Drawing::Point(233, 57);
			this->lbl_slimness_threshold->Name = L"lbl_slimness_threshold";
			this->lbl_slimness_threshold->Size = System::Drawing::Size(13, 13);
			this->lbl_slimness_threshold->TabIndex = 15;
			this->lbl_slimness_threshold->Text = L"0";
			// 
			// slimness_threshold_trackbar
			// 
			this->slimness_threshold_trackbar->Location = System::Drawing::Point(13, 57);
			this->slimness_threshold_trackbar->Maximum = 1000;
			this->slimness_threshold_trackbar->Name = L"slimness_threshold_trackbar";
			this->slimness_threshold_trackbar->Size = System::Drawing::Size(213, 42);
			this->slimness_threshold_trackbar->TabIndex = 14;
			this->slimness_threshold_trackbar->TickFrequency = 50;
			this->slimness_threshold_trackbar->Scroll += gcnew System::EventHandler(this, &bsp_tree_panel::slimness_threshold_trackbar_Scroll);
			// 
			// chb_portals
			// 
			this->chb_portals->AutoSize = true;
			this->chb_portals->Location = System::Drawing::Point(13, 201);
			this->chb_portals->Name = L"chb_portals";
			this->chb_portals->Size = System::Drawing::Size(85, 17);
			this->chb_portals->TabIndex = 13;
			this->chb_portals->Text = L"Draw portals";
			this->chb_portals->UseVisualStyleBackColor = true;
			this->chb_portals->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_portals_CheckedChanged);
			// 
			// chb_splitted_blanks
			// 
			this->chb_splitted_blanks->AutoSize = true;
			this->chb_splitted_blanks->Cursor = System::Windows::Forms::Cursors::Default;
			this->chb_splitted_blanks->Location = System::Drawing::Point(12, 177);
			this->chb_splitted_blanks->Name = L"chb_splitted_blanks";
			this->chb_splitted_blanks->Size = System::Drawing::Size(121, 17);
			this->chb_splitted_blanks->TabIndex = 12;
			this->chb_splitted_blanks->Text = L"Draw splitted blanks";
			this->chb_splitted_blanks->UseVisualStyleBackColor = true;
			this->chb_splitted_blanks->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_splitted_blanks_CheckedChanged);
			// 
			// chb_portal_blanks
			// 
			this->chb_portal_blanks->AutoSize = true;
			this->chb_portal_blanks->Location = System::Drawing::Point(12, 153);
			this->chb_portal_blanks->Name = L"chb_portal_blanks";
			this->chb_portal_blanks->Size = System::Drawing::Size(114, 17);
			this->chb_portal_blanks->TabIndex = 11;
			this->chb_portal_blanks->Text = L"Draw portal blanks";
			this->chb_portal_blanks->UseVisualStyleBackColor = true;
			this->chb_portal_blanks->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_portal_blanks_CheckedChanged);
			// 
			// chb_splitted_geometry
			// 
			this->chb_splitted_geometry->AutoSize = true;
			this->chb_splitted_geometry->Location = System::Drawing::Point(13, 129);
			this->chb_splitted_geometry->Name = L"chb_splitted_geometry";
			this->chb_splitted_geometry->Size = System::Drawing::Size(133, 17);
			this->chb_splitted_geometry->TabIndex = 10;
			this->chb_splitted_geometry->Text = L"Draw splitted geometry";
			this->chb_splitted_geometry->UseVisualStyleBackColor = true;
			this->chb_splitted_geometry->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_splitted_geometry_CheckedChanged);
			// 
			// chb_geometry
			// 
			this->chb_geometry->AutoSize = true;
			this->chb_geometry->Checked = true;
			this->chb_geometry->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chb_geometry->Location = System::Drawing::Point(13, 105);
			this->chb_geometry->Name = L"chb_geometry";
			this->chb_geometry->Size = System::Drawing::Size(97, 17);
			this->chb_geometry->TabIndex = 9;
			this->chb_geometry->Text = L"Draw geometry";
			this->chb_geometry->UseVisualStyleBackColor = true;
			this->chb_geometry->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_geometry_CheckedChanged);
			// 
			// chb_draw_divider
			// 
			this->chb_draw_divider->AutoSize = true;
			this->chb_draw_divider->Location = System::Drawing::Point(149, 177);
			this->chb_draw_divider->Name = L"chb_draw_divider";
			this->chb_draw_divider->Size = System::Drawing::Size(85, 17);
			this->chb_draw_divider->TabIndex = 8;
			this->chb_draw_divider->Text = L"Draw divider";
			this->chb_draw_divider->UseVisualStyleBackColor = true;
			this->chb_draw_divider->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_draw_divider_CheckedChanged);
			// 
			// chb_double_sided
			// 
			this->chb_double_sided->AutoSize = true;
			this->chb_double_sided->Location = System::Drawing::Point(149, 153);
			this->chb_double_sided->Name = L"chb_double_sided";
			this->chb_double_sided->Size = System::Drawing::Size(88, 17);
			this->chb_double_sided->TabIndex = 7;
			this->chb_double_sided->Text = L"Double sided";
			this->chb_double_sided->UseVisualStyleBackColor = true;
			this->chb_double_sided->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_double_sided_CheckedChanged);
			// 
			// chb_solid
			// 
			this->chb_solid->AutoSize = true;
			this->chb_solid->Location = System::Drawing::Point(150, 129);
			this->chb_solid->Name = L"chb_solid";
			this->chb_solid->Size = System::Drawing::Size(49, 17);
			this->chb_solid->TabIndex = 6;
			this->chb_solid->Text = L"Solid";
			this->chb_solid->UseVisualStyleBackColor = true;
			this->chb_solid->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_solid_CheckedChanged);
			// 
			// chb_aabb
			// 
			this->chb_aabb->AutoSize = true;
			this->chb_aabb->Checked = true;
			this->chb_aabb->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chb_aabb->Location = System::Drawing::Point(149, 105);
			this->chb_aabb->Name = L"chb_aabb";
			this->chb_aabb->Size = System::Drawing::Size(118, 17);
			this->chb_aabb->TabIndex = 5;
			this->chb_aabb->Text = L"Draw bounding box";
			this->chb_aabb->UseVisualStyleBackColor = true;
			this->chb_aabb->CheckedChanged += gcnew System::EventHandler(this, &bsp_tree_panel::chb_aabb_CheckedChanged);
			// 
			// lbl_min_square
			// 
			this->lbl_min_square->AutoSize = true;
			this->lbl_min_square->Location = System::Drawing::Point(233, 17);
			this->lbl_min_square->Name = L"lbl_min_square";
			this->lbl_min_square->Size = System::Drawing::Size(13, 13);
			this->lbl_min_square->TabIndex = 2;
			this->lbl_min_square->Text = L"0";
			// 
			// min_square_trackbar
			// 
			this->min_square_trackbar->Location = System::Drawing::Point(12, 8);
			this->min_square_trackbar->Maximum = 1000;
			this->min_square_trackbar->Name = L"min_square_trackbar";
			this->min_square_trackbar->Size = System::Drawing::Size(214, 42);
			this->min_square_trackbar->TabIndex = 0;
			this->min_square_trackbar->TickFrequency = 50;
			this->min_square_trackbar->Scroll += gcnew System::EventHandler(this, &bsp_tree_panel::min_square_trackbar_Scroll);
			// 
			// bsp_node_save_dialog
			// 
			this->bsp_node_save_dialog->DefaultExt = L"lua";
			this->bsp_node_save_dialog->Filter = L"lua | *.lua";
			this->bsp_node_save_dialog->RestoreDirectory = true;
			// 
			// bsp_node_open_dialog
			// 
			this->bsp_node_open_dialog->Filter = L"lua | *.lua";
			this->bsp_node_open_dialog->RestoreDirectory = true;
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(164, 6);
			// 
			// bsp_tree_panel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 747);
			this->Controls->Add(this->bsp_tree_view);
			this->Controls->Add(this->panel1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->Name = L"bsp_tree_panel";
			this->Text = L"BSP tree view";
			this->Shown += gcnew System::EventHandler(this, &bsp_tree_panel::on_bsp_tree_panel_shown);
			this->bsp_tree_context_menu->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->position_difference_trackbar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->square_difference_trackbar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->slimness_threshold_trackbar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->min_square_trackbar))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
		bsp_tree*		m_bsp_tree;
		System::String^ m_model_name;
		System::Void bsp_tree_view_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e);
		System::Void min_square_trackbar_Scroll(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_aabb_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_solid_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void serializeNodeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void serialiseWholeTreeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void loadWholeTreeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_double_sided_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_draw_divider_CheckedChanged(System::Object^  sender, System::EventArgs^  e);

		System::Void chb_geometry_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_splitted_geometry_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_portal_blanks_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_splitted_blanks_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_portals_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void slimness_threshold_trackbar_Scroll(System::Object^  sender, System::EventArgs^  e);
		System::Void chb_outer_edges_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void square_difference_trackbar_Scroll(System::Object^  sender, System::EventArgs^  e);
		System::Void position_difference_trackbar_Scroll(System::Object^  sender, System::EventArgs^  e);
		System::Void on_bsp_tree_panel_shown(System::Object^  sender, System::EventArgs^  e);
};
}//namespace model_editor
}// namespace xray
