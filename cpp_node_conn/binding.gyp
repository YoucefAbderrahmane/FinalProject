{
  "targets": [
    {
      "target_name": "scheduler",
      "sources": [#"../scheduler/Model/Observation.cpp",
      				"../scheduler/Model/Target.cpp",
      				#"../scheduler/Model/Request.cpp",
      				#"../scheduler/Model/Obsconditions.cpp",
      				#"../scheduler/Model/targettest.cpp",
              "test_wrap.cxx"],
      "include_dirs": ["../scheduler/Model", "/home/pfe/dev/Cpp/Libraries/libnova-0.15.0/src/"]
    }
  ]
}