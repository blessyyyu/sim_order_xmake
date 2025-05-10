add_rules("mode.debug", "mode.release")

add_requires("redis-plus-plus")
target("redispp")
    set_kind("static")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_deps("utils")
    add_packages("redis-plus-plus")
