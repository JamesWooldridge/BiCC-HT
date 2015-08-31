#
#	Generates graphs of varying sizes using graphGen.c
#

import os

graphSizes = {
	10: 		[1, 2],
	100: 		[1, 10, 25, 50],
	1000:		[1, 10, 25, 50, 100, 250, 500],
	10000:		[10, 25, 50, 200, 250, 500, 1000, 2500, 5000],
	100000:		[10, 25, 50, 200, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000],
	1000000:	[10, 25, 50, 200, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000, 100000, 250000, 500000]
}

for n in sorted(graphSizes.iterkeys()):
	for c in graphSizes[n]:
		graphFile = "input/{0}-{1}".format(n, c);
		bicFile = "input/{0}-{1}.bic".format(n, c);
		print "./graphgen {0} {1} {2} {3}".format(n, c, graphFile, bicFile)
		os.system("./graphgen {0} {1} {2} {3}".format(n, c, graphFile, bicFile))