num = 0

og = list(filter(None, open("input", "r").readlines()))

for i in range(1, 51):
    last = "%d,%d,%d\n" % (i, i, i)
    with open("%d.in" % i, "w") as f:
        f.writelines(og)
        f.write(last)
