require "rake/testtask"
require 'rake/extensiontask'
require "bundler/gem_tasks"

task :default => :test

Rake::TestTask.new do |t|
  t.libs << "test"
  t.warning = true
end

task :test_isolated do
  Dir["test/test*.rb"].each do |fn|
    ruby "-Ilib:test", "-w", fn
    abort unless $?.success?
  end
end

Rake::ExtensionTask.new('sprockets_ext') do |ext|
  ext.lib_dir = "lib/sprockets"
end
