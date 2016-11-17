class TypeProperty < ActiveRecord::Base
  attr_accessible :category, :entity_type_id, :name, :values

  belongs_to :entity_type, :counter_cache => true

  CATEGORY = {
    "MANUAL" => "Input values manually", 
    "SELECT" => "Select from pre-defined set of values", 
    "INPUT" => "Select or Input values"
  }
end
