machines = [x.strip() for x in open("machinefile", "r").readlines()]

cores_per_machine = 10
total_cores = 8

while total_cores <= 32:
    entry_list = []

    for i in range(total_cores):
        machine = i // cores_per_machine
        core = i % cores_per_machine
        entry = machines[machine] + " slot=0:" + str(core)
        entry_list.append(entry)

    example = "rank 0=soctf-pdf-003 slot=0:0"

    with open(str(total_cores) + "rankfile", "w") as f:
        for i in range(65):
            entry = entry_list[i % len(entry_list)]
            f.write("rank %d=" % (i) + entry + "\n")

    total_cores *= 2
