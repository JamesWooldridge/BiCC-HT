#
#	Runs the bic algorithm on each of the below graphs, then uses the networkx validation method
#	to validate the outputted components
#

import os
import validate

graphSizes = {
	10: 		[1, 2],
	100: 		[1, 10, 25, 50],
	1000:		[1, 10, 25, 50, 200, 250, 500],
	10000:		[10, 25, 50, 200, 250, 500, 1000, 2500, 5000],
	100000:		[10, 25, 50, 200, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000],
	1000000:	[10, 25, 50, 200, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000, 100000, 250000, 500000]

}

for n in sorted(graphSizes.iterkeys()):
	for c in graphSizes[n]:
		inputFile = "input/{0}-{1}".format(n, c);
		outputFile = "output/{0}-{1}.bic".format(n, c);
		correctOutput = "input/{0}-{1}.bic".format(n, c);

		# Run bic
		os.system("./bic {0} {1} 1".format(inputFile, outputFile))
		
		# Validate output
		validate.graph_match(correctOutput, outputFile)
	
	print ""