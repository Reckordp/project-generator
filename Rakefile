SUMBER = Dir.glob("src/*.c")
TUJUAN = "project.exe"
INCLUDE_JALUR = %w( include )#C:/Ruby33-x64/msys64/usr/include )
BEFRAYOPTS = ''#"-L . -lbiji"

OBJFILE = SUMBER.collect do |s|
  ext = s.sub(/\.c$/, '.o')
  File.join('build', File.basename(ext))
end
GETSRC = proc do |task_name|
  ext = task_name.sub(/.o$/, '.c')
  File.join('src', File.basename(ext))
end

rule '.o' => GETSRC do |t|
  include_opt = INCLUDE_JALUR.collect { |i| "-I " + i } .join(' ')
  sh "gcc -g #{include_opt} -o #{t.name} -c #{t.prerequisites.first}"
end

file TUJUAN => OBJFILE do
  # sh 'windres -i src/jaornad.rc -O coff -o build/jaornad.res'
  sh "gcc #{OBJFILE.join(" ")} #{BEFRAYOPTS} -o #{TUJUAN}"
end

task :clean do 
  sh 'rmdir /S /Q build'
  sh 'del ' + TUJUAN
end

Dir.mkdir('build') unless Dir.exist?('build')
task default: TUJUAN