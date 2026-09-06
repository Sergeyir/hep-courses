{
   gInterpreter->AddIncludePath("include");
   gInterpreter->AddIncludePath("yaml-cpp/include");
   
   gSystem->Load("lib/libyaml-cpp.so");
}
