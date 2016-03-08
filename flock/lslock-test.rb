#!/usr/bin/ruby

require 'optparse'


# default options
numberthreads  = 5
sleeptime = 30
banner = "Usage:  #{__FILE__} <arguments>"

optparse  = OptionParser.new do |opts|
  opts.banner = banner
  opts.separator ''
  opts.separator 'Optional Arguments:'

  opts.on('-n', '--numPIDs <integer>',
          'number of PIDs to create (default: 5)') do |num_locks|
    numberthreads = num_locks.to_i
  end
  opts.on('-t', '--timesleep <integer>',
          'number of seconds (default: 30)') do |seconds|
    sleeptime = seconds.to_i
  end
end
optparse.parse!

warn "Total PIDs:   #{numberthreads.inspect}"
warn "Sleeptime: #{sleeptime.inspect}'s"

dir = '/tmp/lslock-test'

Dir.mkdir(dir) unless Dir.exist?(dir)

numberthreads.times do
    fork do
       puts "PID #{Process.pid}"
       fullname = File.join(dir,"lslock-test_PID-#{Process.pid}.lock")
         File.open(fullname, File::RDWR|File::CREAT, 0666) {|f|
           f.flock(File::LOCK_EX)
           sleep sleeptime
           File.delete(f)
        }
    end
end
