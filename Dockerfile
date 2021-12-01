FROM corgibytes/ruby-1.9.3:1.0.0

# Working folder.
RUN mkdir /app
WORKDIR /app
COPY . .

# Install Gems.
RUN bundle install