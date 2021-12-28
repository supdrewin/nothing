#pragma once

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "database.hh"
#include "user.hh"

#include "db/items.hpp"

#include "tools/platform.hh"
#include "tools/secure.hh"
#include "tools/types.hh"
#include "tools/vaild.hh"

struct user_db : public database<user_data> {
  std::string filename;
  items its;

  user_db(const char *name = "${database}") : filename(name), its() {
    this->read();
  }

  void add_user(user_identity id = student) {
    user_data tmp{id};

  insert_username:
    printf("username: ");
    std::cin >> tmp.u.username;

    if (find_username(tmp.u.username) != -1)
      goto insert_username;

  insert_number:
    printf("number: ");
    check_cin(tmp.num);

    if (find_number(tmp.num) != -1)
      goto insert_number;

    printf("scores:\n");
    tmp.sc.resize(its.size() + 1);
    for (size_t i{}; i < its.size();) {
      printf("%s: ", its[i].c_str());
      check_cin(tmp.sc[++i]);
      tmp.sc[0] += tmp.sc[i];
    }

    tmp.u.passwd = tmp.u.username;
    data.push_back(tmp);
  }

  void add_item(std::string it) {
    its.add(it);
    size_t score;
    for (auto &_ : data) {
      _.u.id ? (printf("%s: ", _.u.username.c_str()), check_cin(score),
                _.sc[0] += score, _.sc.add(score))
             : _.sc.add(0);
    }
  }

  bool del_item(std::string it) {
    for (size_t i{}; i < its.size(); ++i)
      if (it == its[i].data()) {
        its.erase(i);
        for (auto &_ : data) {
          _.sc[0] -= _.sc[i + 1];
          _.sc.erase(i + 1);
        }
        return true;
      }
    return false;
  }

  index_t find_username(std::string name) {
    index_t i{};
    for (auto _ : data) {
      if (name == _.u.username)
        return i;
      ++i;
    }
    return -1;
  }

  index_t find_number(size_t num) {
    index_t i{};
    for (auto _ : data) {
      if (num == _.num)
        return i;
      ++i;
    }
    return -1;
  }

  std::vector<index_t> find_score(size_t sc) {
    std::vector<index_t> tmp;
    index_t i{};
    for (auto _ : data) {
      if (_.u.id and sc == _.sc[0])
        tmp.push_back(i);
      ++i;
    }
    return tmp;
  }

  void write() {
    its.write();
    std::ofstream ofs(filename);

    ofs << "username\tpasswd\tnumber\tid\tscores";
    bool default_admin{false};

    for (auto _ : data) {
      ofs << '\n'
          << _.u.username << '\t' << secure::write(_.u.passwd) << '\t' << _.num
          << '\t' << _.u.id;

      for (auto __ : _.sc)
        ofs << '\t' << __;

      if (not default_admin)
        default_admin = (_.u.username == "root");
    }

    if (not default_admin) {
      ofs << "\nroot\t" << secure::write("root") << "\t0\t0";
      for (size_t i{}; i < its.size(); ++i)
        ofs << "\t0";
    }
  }

  void read() {
    its.read(), this->clear();

    std::ifstream ifs(filename);
    if (not ifs.good())
      this->write(), ifs.open(filename);

    std::string header[5];
    for (auto &_ : header)
      ifs >> _;

    (header[0] == "username" and header[1] == "passwd" and
     header[2] == "number" and header[3] == "id" and header[4] == "scores")
        ? void(0)
        : exit(1);

    user_data tmp;
    for (size_t i{}; not ifs.eof(); ++i) {
      ifs >> tmp.u.username >> tmp.u.passwd >> tmp.num >> tmp.u.id;
      tmp.sc.resize(its.size() + 1);
      for (size_t j{}; j < its.size() + 1; ++j)
        ifs >> tmp.sc[j];
      secure::read(tmp.u.passwd);
      data.push_back(tmp);
    }
  }

  void print_user(size_t i) {
    printf("|%9s  |%11d |", data[i].u.username.c_str(),
           static_cast<int>(data[i].num));
    this->print_scores(i);
  }

  void print_scores(size_t i) {
    for (size_t j{}; j < its.size() + 1; ++j)
      printf("%8d  |", static_cast<int>(data[i].sc[j]));
    printf("\n");
  }
};
