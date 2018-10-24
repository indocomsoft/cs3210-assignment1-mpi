import os
from collections import defaultdict

STOPPED = 0
MOVING = 1

line_colors = ['g', 'y', 'b']


def validate():
    files = list(filter(lambda x: x.endswith(".out"), os.listdir(".")))
    for f in files:
        validate_one(f)


def validate_one(f):
    print(f)
    train_history = defaultdict(list)
    prefix = f.split(".")[0]
    contents = list(filter(lambda x: x, open(f, "r").readlines()))[:-3]
    for row in contents:
        validate_row(row, train_history)

    input_contents = list(filter(lambda x: x, open(prefix, "r").readlines()))

    num_stations, station_map, lines, num_trains, start_train_ids = parse_input(
        input_contents)

    for k in sorted(sorted(train_history.keys(), key=lambda x: int(x[1:])), key=lambda x: x[0]):
        # print(k)
        line_id = line_colors.index(k[0])
        train_id = int(k[1:])

        if (train_id - start_train_ids[line_id]) % 2 == 0:
            reverse = True
        else:
            reverse = False

        validate_train_steps(
            train_history[k], lines[line_id], station_map, reverse)

        # for i, entry in enumerate(train_history[k]):
        #     print(entry)


def validate_train_steps(steps, line, station_map, reverse):
    prev = int(steps[0][1][1:])
    next_type = tuple

    for i, val in enumerate(steps[1:], 1):
        _, step, duration = val

        if (type(step)) != next_type:
            print("train did not follow stop-start-stop pattern")

        if len(step) == 2 and type(step) == tuple:
            src, dst = list(map(lambda x: int(x[1:]), step))
            if src != prev:
                print("Departed location does not match previous stop")

            elif dst != find_next_station(line, src, reverse):
                print("Train on wrong edge")

            elif station_map[src][dst] != duration and i != len(steps) - 1:
                print("Train did not stop for correct duration")
            else:
                prev = dst
                next_type = str

        else:
            cur = int(step[1:])
            if cur != prev:
                print("Train did not wait at edge dst")
            if cur == line[-1] and reverse == False:
                reverse = True
            elif cur == line[0] and reverse == True:
                reverse = False

            next_type = tuple


def find_next_station(line, val, reverse):
    if reverse:
        search_line = line[::-1]
    else:
        search_line = line

    for i, x in enumerate(search_line):
        if x == val:
            return search_line[i+1]


def parse_input(contents):
    contents = [r.strip() for r in contents]
    num_stations = int(contents[0])
    station_names = contents[1].split(",")
    station_map = []
    for i in range(num_stations):
        r = contents[2+i]
        station_map.append([int(x) for x in r.split()])

    lines = []

    for i in range(3):
        line_names = contents[-5+i].split(",")
        line_idx = list(map(lambda x: station_names.index(x), line_names))
        lines.append(line_idx)

    num_trains = [int(x) for x in contents[-1].split(",")]

    start_train_ids = num_trains
    start_train_ids[1] += num_trains[0]
    start_train_ids[2] += num_trains[1]

    return num_stations, station_map, lines, num_trains, start_train_ids


def validate_row(row, train_history):
    time, records = [c.strip() for c in row.split(":")]
    edges_used = set()
    records = list(filter(lambda x: x, [c.strip()
                                        for c in records.split(",")]))
    for r in records:
        status, train, res = parse_record(r)
        if status == MOVING:
            if res in edges_used:
                print("Error: edge sharing")
                print(row)
                print(r)
                exit()
            else:
                edges_used.add(res)

        hist = train_history[train]
        if len(hist) == 0 or hist[-1][1] != res:
            hist.append([time, res, 1])
        else:
            hist[-1][2] += 1


def parse_record(record):
    train, state = record.split("-", 1)

    if len(state.split("->")) == 2:
        status = MOVING
        res = tuple(state.split("->"))
    else:
        status = STOPPED
        res = state

    return status, train, res


if __name__ == "__main__":
    validate()
