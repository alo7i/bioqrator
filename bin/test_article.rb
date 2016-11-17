require 'yajl/json_gem'
require 'tire'
URL = "http://localhost:9200"

no = 0


file = File.new("/var/tmp/bioq/pie_articles", "r")
#file = File.new("test_pie_articles1", "r")
while ((line = file.readline))
  no += 1
  begin
    data = line.split("\t")
  rescue => e 
    puts "ERROR #{no}: #{line}"
    puts e.message
    next
  end
  
end


