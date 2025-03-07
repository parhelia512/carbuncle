MRuby::Gem::Specification.new('mruby-bin-playground') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Ramiro Rojo'
  spec.summary = 'Carbuncle Playground (for the web)'

  spec.bins = %w[playground]

  spec.add_dependency 'carbuncle-core'
  spec.add_dependency 'carbuncle-audio'
  spec.add_dependency 'carbuncle-graphics'
  spec.add_dependency 'carbuncle-input'
  spec.add_dependency 'carbuncle-math'

  spec.add_dependency('mruby-compiler', core: 'mruby-compiler')
  spec.add_dependency('mruby-error', core: 'mruby-error')
  spec.add_test_dependency('mruby-print', core: 'mruby-print')
end
