#! /usr/bin/env ruby
# 
# ./embed-mrb.rb MICROBIT.hex mrbapp.mrb 00060000
#

# constants
DEF_START_ADDR = 0x00060000   # default start address to embed
LINE_BYTES = 16               # bytes per line
LINE_TERM = "\r\n"
FNAME_SUFFIX = "-mruby"

# get arguments
if ARGV.length < 2
  puts "Usage: #{$0} hex mrb [addr]"
  puts "  hex: Target Intel HEX file."
  puts "  mrb: MRB (mruby binary) file to embed."
  puts " addr: Start address to embed MRB. (default: 0x#{DEF_START_ADDR.to_s(16)})"
  exit 0
end
$src_hex = ARGV[0]  # Source HEX file name
$dst_hex = $src_hex.dup
$dst_hex[-File.extname($dst_hex).length, 0] = FNAME_SUFFIX        # Dest. HEX file
$mrb_file = ARGV[1] # MRB file name
$start_addr = ARGV.length > 2 ? ARGV[2].to_i(16) : DEF_START_ADDR # Start address

# add_checksum(bin) #=> Array
# <args>
#   bin: Binary array to calculate checksum
# <return>
#   binaries array
def add_checksum(bin)
  sum = bin.inject(:+)  # bin.sum (Ruby 2.5~)
  bin << (0x100 - (sum & 0xff))
end

# hex_line(bin) #=> String
# <args>
#   bin: Binary array to change HEX string
# <return>
#   HEX string with line termination
def hex_line(bin)
  hex = ":"
  bin.each {|b| hex += sprintf("%02X", b)}
  hex + LINE_TERM
end

# make_segment_header(addr) #=> String
# <args>
#   addr: segment address
# <return>
#   HEX string of segment header
def make_segment_header(addr)
  #      len,  adrH, ADRL, seg
  bin = [0x02, 0x00, 0x00, 0x02]
  addr >>= 4
  bin << ((addr >> 8) & 0xff) # Segment address (H)
  bin << (addr & 0xff)        # Segment address (L)
  add_checksum(bin)
  hex_line(bin)
end

# # segment address record
# bin = []
# bin << 0x02 # length
# bin << 0x00 # addr(H)
# bin << 0x00 # addr(L)
# bin << 0x02 # segment address
# bin << (($start_addr >> 12) & 0xff)
# bin << (($start_addr >> 4) & 0xff)
# # sum = bin.inject(:+)
# # bin << 0x100 - (sum & 0xff)
# add_checksum(bin)
# p bin
# p hex = hex_line(bin)

# get_mrb_hex(mrb_file, addr) #=> String
# <args>
#   mrb_file: MRB file name.
#   addr:     Destination address for MRB.
# <return>
#   Intel HEX string of MRB.
def get_mrb_hex(mrb_file, addr)
  # segment header
  hex = make_segment_header(addr)
  # p hex

  File.open(mrb_file, 'rb') {|f|
    offset = 0
    until f.eof?
      chunk = f.read(LINE_BYTES)
      #      length,       seg addr(H),          seg addr(L),   data    bin
      bin = [chunk.length, (offset >> 8) & 0xff, offset & 0xff, 0x00] + chunk.bytes
      add_checksum(bin)
      # p bin
      hex += hex_line(bin)
      offset += LINE_BYTES
    end
  }
  hex
rescue => e
  p e
end

# # mrb
# mrb = File.open(ARGV[0], 'rb') {|f|
#   offset = 0 
#   until f.eof?
#     bin = f.read(LINE_BYTES)
#     p bin

#     bin = [bin.length, (offset >> 8) & 0xff, offset & 0xff, 0x00] + bin.bytes
#     add_checksum(bin)
#     hex += hex_line(bin)

#     offset += LINE_BYTES
#   end
# }

# convert MRB to HEX
mrb_hex = get_mrb_hex($mrb_file, $start_addr)

# Read source HEX file
hex_lines = File.readlines($src_hex)

# Write destination HEX file
outhex = File.open($dst_hex, "w")
outhex.write hex_lines[0..-2].join  # original HEX without end record
outhex.write mrb_hex                # MRB segment
outhex.write hex_lines[-1]          # end record
outhex.close
