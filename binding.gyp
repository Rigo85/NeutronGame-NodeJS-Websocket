{
	"targets": [{
		"target_name": "NativeMinimax",
		"include_dirs" : [
			"games/neutron/native/include",
			"<!(node -e \"require('nan')\")"
		],
		"sources": [
			"games/neutron/native/src/Board.cpp",
            "games/neutron/native/src/cleaners.cpp",
            "games/neutron/native/src/FullMove.cpp",
            "games/neutron/native/src/gameutils.cpp",
            "games/neutron/native/src/minimax.cpp",
            "games/neutron/native/src/Move.cpp",
            "games/neutron/native/src/MinimaxAsyncWorker.cpp",
            "games/neutron/native/src/MinimaxAsyncBinding.cpp",
            "games/neutron/native/index.cpp"
		],
        "cflags_cc": [
            "-Wno-cast-function-type",
            "-Wall",
            "-std=c++17",
            "-O3",
            "-flto", 
            "-ffast-math",
            "-funroll-all-loops",
            "-msse4.2",
            "-march=native",
            "-mtune=native"
        ]
	}]
}
