def parse_line_stat(line)
  line.match(/-> (.*)/).captures.first.split(', ').map(&:to_f)
end

puts "process-edge/thread-train,thread/num_core,short,time,green_avg,green_max,green_min,yellow_avg,yellow_max,yellow_min,blue_avg,blue_max,blue_min"

Dir.glob("*.out").each do |filename|
  short = filename.include? "short"
  content = File.read(filename).split("\n")
  lines_timing =
    ["green", "yellow", "blue"].flat_map do |c|
      parse_line_stat content.find { |line| line.start_with? c }
    end
    .join(",")
  time =
    content
    .find { |line| line.start_with? "real" }
    .delete_prefix("real\t")
    .delete_suffix("s")
    .split("m")
    .map(&:to_f)
    .reduce(0) { |acc, t| acc * 60 + t }
  if filename.include? "thread"
    edge_train = filename.split("_").first
    num_core = "thread"
  else
    edge_train, num_core = filename.match(/(\d*)_.*\.(\d*)\..*/).captures
  end
  puts "#{edge_train},#{num_core},#{short},#{time},#{lines_timing}"
end
