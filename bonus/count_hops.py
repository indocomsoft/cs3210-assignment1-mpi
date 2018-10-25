import os
from collections import defaultdict
import sys


def count():
    files = list(filter(lambda x: x.endswith(".out"), os.listdir(".")))
    for f in files:
        fn = f[:-4]
        res = count_one(f)
        print(fn, res)


def count_one(f):
    train_history = defaultdict(list)
    contents = list(filter(lambda x: x, open(f, "r").readlines()))[:-3]
    for row in contents:
        parse_row(row, train_history)

    count = 0

    for k in train_history.keys():
        for _, step, _ in train_history[k]:
            if type(step) == tuple and len(step) == 2:
                count += 1

    return count


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

    start_train_ids = [0 for i in range(3)]
    start_train_ids[1] = num_trains[0]
    start_train_ids[2] = num_trains[1] + start_train_ids[1]

    return num_stations, station_map, lines, num_trains, start_train_ids


def parse_row(row, train_history):
    time, records = [c.strip() for c in row.split(":")]
    records = list(filter(lambda x: x, [c.strip()
                                        for c in records.split(",")]))
    for r in records:
        train, res = parse_record(r)
        hist = train_history[train]
        if len(hist) == 0 or hist[-1][1] != res:
            hist.append([time, res, 1])
        else:
            hist[-1][2] += 1


def parse_record(record):
    train, state = record.split("-", 1)

    if len(state.split("->")) == 2:
        res = tuple(state.split("->"))
    else:
        res = state

    return train, res


if __name__ == "__main__":
    count()
    # count_one(sys.argv[1])
