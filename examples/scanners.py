import argparse

import flowtuple


def conv(ip):
    """
    Converts IP integer to human readable IP address
    :param ip: integer IP Address in int form
    :return: string IP Address
    """
    return "%d.%d.%d.%d" % (
        (ip >> 24) & 255,
        (ip >> 16) & 255,
        (ip >> 8) & 255,
        ip & 255,
    )


def earliest_ts(data):
    ret = None
    for x in data.values():
        if ret is None or x[0] < ret:
            ret = x[0]
    return ret


def latest_ts(data):
    ret = None
    for x in data.values():
        if ret is None or x[-1] > ret:
            ret = x[-1]
    return ret


def housekeeping(tracker, current_ts, inter, key):
    x = tracker[key]
    for y in list(x.keys()):
        while len(x[y]) > 0 and x[y][0] < current_ts - inter:
            del x[y][0]
        if len(x[y]) == 0:
            del x[y]


def process_data(scanners, data, tracker, ts, inter):
    if data.protocol == 1 and data.src_port != 8:
        return

    if data.protocol == 1:
        port = 0
    else:
        port = data.dest_port
    key = (data.src_ip, port, data.protocol,)

    if key in scanners:
        return

    if key not in tracker:
        tracker[key] = {}

    if data.dest_ip not in tracker[key]:
        tracker[key][data.dest_ip] = [ts]
    else:
        tracker[key][data.dest_ip].append(ts)

    housekeeping(tracker, ts + 60, inter, key)

    if len(tracker[key]) >= 25:
        val1 = earliest_ts(tracker[key])
        val2 = latest_ts(tracker[key])
        print("{}|{}|{}|{}|{}|{}".format(conv(key[0]), key[1], key[2], val1, val2, 25))
        scanners.add(key)
        del tracker[key]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="determine scanners in a set of flowtuple files")
    parser.add_argument('ftfns', type=str, metavar='flowtuple_file', nargs='+',
                        help='A flowtuple file to analyze from')
    parser.add_argument('-s', '--src-filter', dest='src_filter', action='store',
                        default=None, help='File that contains desired IP address ranges for source')
    parser.add_argument('-d', '--dest-filter', dest='dst_filter', action='store',
                        default=None, help='File that contains desired IP address ranges for destination')
    parser.add_argument('-i', dest='inter_len', action='store',
                        default=300, help='Interval length (in seconds)')
    parser.add_argument('-m', dest='threshold', action='store',
                        default=25, help='Threshold before IP is counted as a scanner')

    args = parser.parse_args()

    if args.src_filter is not None:
        pass

    if args.dst_filter is not None:
        pass

    current_inter_ts = 0
    tracker_dict = {}
    found_scanners = set()

    handle = None
    for ftfn in args.ftfns:
        try:
            handle = flowtuple.Handle(ftfn)
        except flowtuple.error as e:
            if e.args[1] == 4:
                print("Could not open '{}' for reading... Aborting...".format(ftfn))
                exit(1)
            exit(-1)

        record = handle.get_next()
        while record is not None:
            if isinstance(record, flowtuple.Interval):
                if record.time < current_inter_ts:
                    print("Files not in order... Aborting...")
                    exit(2)
                else:
                    current_inter_ts = record.time

            elif isinstance(record, flowtuple.Data):
                if record.protocol == 1 or record.protocol == 6 or record.protocol == 17:
                    process_data(found_scanners, record, tracker_dict, current_inter_ts, args.inter_len)

            record = handle.get_next()
