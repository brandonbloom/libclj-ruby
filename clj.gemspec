$:.push File.expand_path('../lib', __FILE__)
require 'clj/version'

Gem::Specification.new do |gem|

  gem.name = 'clj'
  gem.version = CLJ::VERSION
  gem.platform = Gem::Platform::RUBY
  gem.summary = 'Reads and prints Clojure forms'
  gem.description = 'Reads and prints Clojure forms.'
  gem.homepage = 'https://github.com/brandonbloom/libclj-rb'
  gem.author = 'Brandon Bloom'
  gem.email = 'brandon@brandonbloom.name'

  gem.files = Dir.glob('lib/**/*.rb') + Dir.glob('ext/**/*.{c,h,rb}')
  gem.extensions = ['ext/clj/extconf.rb']

  gem.add_development_dependency 'rake-compiler'
  gem.add_development_dependency 'rake'

end
