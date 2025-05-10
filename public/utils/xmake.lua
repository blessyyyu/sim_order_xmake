add_rules("mode.debug", "mode.release")

target("utils")
    set_kind("static")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})