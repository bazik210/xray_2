# Set graph settings
SET(GRAPHVIZ_GRAPH_HEADER "\
rankdir=RL;\
ranksep=\"2.0\";\
bgcolor=aliceblue;\
graph [ nodesep=\"0.5\" ];\
node [ height=\"0.9\", width=\"1.2\", fontsize=\"15\", color=black, style=filled, fillcolor=azure2 ];\
edge [ arrowhead=inv, arrowtail=normal, dir=back, arrowsize=\"1.1\", penwidth=\"2\" color=black ];\
")

# Don't generate graphs that we aren't interested in
SET(GRAPHVIZ_GENERATE_DEPENDERS FALSE)
SET(GRAPHVIZ_GENERATE_PER_TARGET FALSE)