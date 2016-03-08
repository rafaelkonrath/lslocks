#!/usr/bin/python


class LsLock:

    def check_locks(self, path):

        try:

            fd_proc_lock = self._open_proc_locks()
            locks = self._read_proc_locks(fd_proc_lock)
            files_path = self._get_filepaths(path)
            results = self._compare_inodes(files_path, locks)
            self._printresults(results)

        except Exception, e:
            print e
            exit(2)

    def _printresults(self, results):

        try:

            for p, f in results.items():
                    print p, f

        except Exception, e:
            print e
            exit(2)

    def _compare_inodes(self, files, locks):

        try:

            matchs = {}

            for f in files:
                inode = os.stat(f).st_ino
                if str(inode) in locks:
                    matchs[locks[str(inode)]] = f

            return matchs


        except Exception, e:
            print e
            exit(2)


    def _read_proc_locks(self, f):

        try:

            locks = {}

            for line in f:
                if not line.strip():
                    continue
                number, standard, exclusive, readwrite, pid, id, start, end  = line.split()
                major, minor, inode = id.split(":")
                locks[inode] = pid

            return locks

        except Exception, e:
            print e
            exit(2)


    def _get_filepaths(self, path):

        try:

            file_path = []

            for root, directories, files in os.walk(path):
                for filename in files:
                    filepath = os.path.join(root, filename)
                    file_path.append(filepath)

            return file_path
        except Exception, e:
            print e
            exit(2)

    def _open_proc_locks(f):

        try:

            f = open('/proc/locks', 'r')
            return f

        except Exception, e:
            print e
            exit(2)


def main( ):

    try:

        if len(sys.argv) == 2:
            lslock = LsLock()
            lslock.check_locks(sys.argv[1])

        else:
            print "Usage :", sys.argv[0], "<dirname>"

    except Exception, e:
        print   e
        sys.exit(2)


if __name__ == "__main__":

    import sys, os
    # Test Profiler
    #import cProfile

    main()
    #cProfile.run('main()')
