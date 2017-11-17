import networkx as nx
import os
import imp

# Assume Decaf is not installed as a package and load directly
wf = imp.load_source('workflow', os.environ['DECAF_PREFIX'] + '/python/workflow.py')

# Instance a graph to represent the workflow
w = nx.DiGraph()

# Add *producer* and *consumer* jobs by adding as nodes to the graph
w.add_node("prod", start_proc=0, nprocs=4, func='prod', cmdline='./jacobi')
w.add_node("con",  start_proc=4, nprocs=4, func='con', cmdline='./jacobi_analyse')

# Add an edge to represent the job coupling
w.add_edge("prod", "con",
           transport="cci",
           start_proc=4, nprocs=0, prod_dflow_redist='count')

# Convert graph workflow and generate config json and shell runner script
wf.processGraph(w, "linear2")
