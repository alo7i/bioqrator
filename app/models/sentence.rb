class Sentence < ActiveRecord::Base
  attr_accessible :content, :offset, :passage_id
end
