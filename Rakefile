require 'rake/clean'
CLEAN.include '**/*.o', '**/*.so', '**/*.bundle', 'pkg', 'tmp'

require 'rake/extensiontask'
Rake::ExtensionTask.new('clj')

require 'bundler/gem_tasks'
task :build => :compile

task :default => %w(compile build)
