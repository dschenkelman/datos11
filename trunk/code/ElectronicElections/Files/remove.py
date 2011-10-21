def remove_empty_lines(fname):
	f = open(fname)
	i = 0
	for l in f:
		i += 1	
		if len(l) == 1:
			print "File: " + fname + ".Line " + str(i) + " empty."

def remove_all():
	remove_empty_lines("candidates.txt")
	remove_empty_lines("charges.txt")
	remove_empty_lines("electionList.txt")
	remove_empty_lines("districts.txt")
	remove_empty_lines("elections.txt")
	remove_empty_lines("padron.txt")

