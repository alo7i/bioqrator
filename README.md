# Bioqrator: a web-based interactive biomedical literature curating system#

## Abstract ##
The time-consuming nature of manual curation and the rapid growth of biomedical literature severely limit the number of articles that database curators can scrutinize and annotate. Hence, semi-automatic tools can be a valid support to increase annotation throughput. Although a handful of curation assistant tools are already available, to date, little has been done to formally evaluate their benefit to biocuration. Moreover, most curation tools are designed for specific problems. Thus, it is not easy to apply an annotation tool for multiple tasks. BioQRator is a publicly available web-based tool for annotating biomedical literature. It was designed to support general tasks, i.e. any task annotating entities and relationships. In the BioCreative IV edition, BioQRator was tailored for protein– protein interaction (PPI) annotation by migrating information from PIE the search. The results obtained from six curators showed that the precision on the top 10 documents doubled with PIE the search compared with PubMed search results. It was also observed that the annotation time for a full PPI annotation task decreased for a beginner-intermediate level annotator. This finding is encouraging because text-mining techniques were not directly involved in the full annotation task and BioQRator can be easily integrated with any text-mining resources.

## Tested systems ##

- Rails: 3.2.22
- MySQL: 5.7.10
- Ruby 1.93
- OS: MacOS X 10.11 (development) and  Ubuntu 14.04.3 LTS (production)


## How to run ##

1. You need to install git, ruby, rails, and MySQL first.
2. Clone this repository: `git clone git@github.com:dongseop/bioqrator.git`
2. `cd bioqrator`
3. `bundle install`
4. You need to create your own 'database.yml' and 'secrets.yml' in 'config' directory. You may refer sample files in the config directory.
4. `rake db:create`
5. `rake db:migrate`
6. `rails s`


## How to test ##

Sorry. Missing test codes.


## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request


## LICENSE

Copyright © 2016, Dongseop Kwon

Released under the Apache License License 2.0
