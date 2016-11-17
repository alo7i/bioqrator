require 'yajl/json_gem'
require 'tire'
URL = "http://localhost:9200"

no = 0
articles = []
Tire.index 'articles' do
  delete
  create
end


puts "#{Time.now} #{no} lines processed"
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
	item = {
		:id => data[0], 
		:type => "article",
		:pmid => data[0],
		:title => (data[1] == "\\N") ? nil : data[1],
		:abstract => (data[2] == "\\N") ? nil : data[2],
		:pub_year => (data[3].nil?) ? nil : data[3].to_i,
		:pub_date => (data[4] == "\\N") ? nil : data[4],
		:source => (data[5] == "\\N") ? nil : data[5],
		:authors => (data[6] == "\\N") ? nil : data[6],
		:ppi_score => (data[7].nil?) ? nil : data[7].to_f 
	}
	articles << item
	if articles.size >= 10000
		Tire.index 'articles' do
		  import articles
		end
		articles = []
		puts "#{Time.now} #{no} lines processed"
	end
end


if articles.size > 0
	Tire.index 'articles' do
	  import articles
	end
	articles = []
end


		puts "#{Time.now} #{no} lines processed"
