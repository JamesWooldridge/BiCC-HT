import networkx as nx
import networkx.algorithms.isomorphism as iso
import numpy as np
import matplotlib.pyplot as plt
import sys

def node_match(n1, n2):
	return n1 == n2

def graph_match(correctGraph, testGraph):
	print "graph"
	correctGraphs = list(); 
	with open(correctGraph, 'r') as f:
		G=nx.Graph()
		for line in f:
			if line.rstrip() == '':
				# graph is over, add to list and init new empty graph
				correctGraphs.append(G)
				G=nx.Graph()
				continue

			line = line.rstrip()
			nodes = line.split("-")
			src = nodes[0]
			G.add_node(src, label = src)
			for dest in nodes[1:]:
				G.add_node(dest, label = dest)
				G.add_edge(src, dest)

	testGraphs = list(); 
	with open(testGraph, 'r') as f:
		G=nx.Graph()
		for line in f:
			if line.rstrip() == '':
				# graph is over, add to list and init new empty graph
				testGraphs.append(G)
				G=nx.Graph()
				continue

			line = line.rstrip()
			nodes = line.split("-")
			src = nodes[0]
			G.add_node(src, label = src)
			for dest in nodes[1:]:
				G.add_node(dest, label = dest)
				G.add_edge(src, dest)

	i = 1
	for testG in testGraphs:
		match = False
		for correctG in correctGraphs:
			if nx.is_isomorphic(testG, correctG, node_match = node_match):
				match = True
		if not match:
			print "\tdifferent"
			sys.exit()
		i = i + 1

	print "\tsame"