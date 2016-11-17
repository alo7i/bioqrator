ret = `rm /var/tmp/bioq/*`

puts "Downloading pie_entities"
ret = `wget 'http://www.ncbi.nlm.nih.gov/CBBresearch/Wilbur/IRET/BIOQRATOR/pie_entities.gz' -P /var/tmp/bioq`
ret = `gunzip /var/tmp/bioq/pie_entities.gz`
puts ret
puts "--------------------------"

puts "Downloading pie_entity_locations"
ret = `wget 'http://www.ncbi.nlm.nih.gov/CBBresearch/Wilbur/IRET/BIOQRATOR/pie_entity_locations.gz' -P /var/tmp/bioq`
ret = `gunzip /var/tmp/bioq/pie_entity_locations.gz`
puts ret
puts "--------------------------"

puts "Downloading pie_articles"
ret = `wget 'http://www.ncbi.nlm.nih.gov/CBBresearch/Wilbur/IRET/BIOQRATOR/pie_articles.gz' -P /var/tmp/bioq`
ret = `gunzip /var/tmp/bioq/pie_articles.gz`
puts ret
puts "--------------------------"



