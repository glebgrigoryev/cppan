/*
 * Copyright (c) 2016, Egor Pugin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of
 *        its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "cppan_string.h"
#include "dependency.h"

struct Config;
class ProjectPath;

struct PackageStore
{
public:
    struct PackageConfig
    {
        Config *config;
        Packages dependencies;
    };
    using PackageConfigs = std::map<Package, PackageConfig>;

    using iterator = PackageConfigs::iterator;
    using const_iterator = PackageConfigs::const_iterator;

public:
    void resolve_dependencies(const Config &c);
    std::tuple<std::set<Package>, Config, String>
        read_packages_from_file(path p, const String &config_name = String(), bool direct_dependency = false);
    bool has_local_package(const ProjectPath &ppath) const;
    void process(const path &p, Config &root);

    Config *add_config(std::unique_ptr<Config> &&config, bool created);
    Config *add_config(const Package &p);
    Config *add_local_config(const Config &c);

    bool rebuild_configs() const { return has_downloads() || deps_changed; }
    bool has_downloads() const { return downloads > 0; }

public:
    PackageConfig &operator[](const Package &p);
    const PackageConfig &operator[](const Package &p) const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    iterator find(const PackageConfigs::key_type &k) { return packages.find(k); }
    const_iterator find(const PackageConfigs::key_type &k) const { return packages.find(k); }

private:
    PackageConfigs packages;
    std::set<std::unique_ptr<Config>> config_store;

    std::set<Package> resolved_packages;
    std::set<ProjectPath> local_packages;

    bool processing = false;
    int downloads = 0;
    bool deps_changed = false;

    void write_index() const;
    void check_deps_changed();

    friend struct Resolver;
};

extern PackageStore rd;
