#pragma once

#define __GY_PLUGIN_DIRECTORY_SELECTOR_VERSION__ L"V1.0_DEV"

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string>
#include <vector>

typedef class DirectorySelector {
public:    DirectorySelector(const std::wstring& path = L"/storage/emulated/0/") noexcept(false);
  virtual ~DirectorySelector(void) noexcept = default;

protected:
  auto update(void) noexcept(false) -> void;
  auto format(std::wstring&) const noexcept -> std::wstring;
  
  std::wstring __path{},
       __recover_path{};
  
  struct stat __stat{};
  dirent* __filename{nullptr};
  DIR*    __dir{nullptr};
  
public:
  // connect
  auto operator/ (std::wstring&& dir_name) noexcept(false) -> DirectorySelector;
  auto operator/=(std::wstring&& dir_name) noexcept(false) -> void;
  auto operator+ (std::wstring&& dir_name) noexcept(false) -> DirectorySelector;
  auto operator+=(std::wstring&& dir_name) noexcept(false) -> void;
  
  // previous
  auto operator- (size_t) noexcept -> DirectorySelector;
  auto operator-=(size_t) noexcept -> void;
  auto operator--(void) noexcept -> void;
  auto operator--(int)  noexcept -> void;
  
  // goto
  auto operator=(std::wstring&& dir_path) noexcept(false) -> void;
  
  // data
//auto operator*(void) noexcept; // Multi-data
  auto operator+(void) noexcept -> std::vector<std::wstring>;
  auto operator-(void) noexcept -> std::vector<std::wstring>;
  
  auto operator& (void)   const noexcept -> std::wstring;
  auto operator[](size_t) const noexcept -> std::wstring;
  auto operator[](std::pair<size_t, size_t>) const noexcept -> std::wstring;
  
  auto count_file(void) noexcept -> size_t;
  
  auto find_partition(size_t) const noexcept -> size_t;
  auto length(void) const noexcept -> size_t;
  
  auto check(std::wstring&& dir_name) const noexcept -> bool;
  auto isPreviousValid(void) noexcept -> bool;
  
  auto recover(void) noexcept -> void;
  
} DirSelector;

//>>>

DirSelector::DirectorySelector(const std::wstring& path) noexcept(false) : __path(path), __recover_path(path) {
  this->update();
}

inline auto DirSelector::update(void) noexcept(false) -> void {
  this->format(this->__path);
  if(this->__path.front() != L'/')
     this->__path = L"/" + this->__path;
  for(size_t pos{this->__path.find(L"//")};
             pos != std::wstring::npos;
             pos = this->__path.find(L"//")) {
    this->__path.replace(pos, 2, L"/");
  }
  
  lstat(std::to_string(this->__path).c_str(), &this->__stat);
    
  if(!S_ISDIR(this->__stat.st_mode))
    throw std::invalid_argument{std::to_string(L"not dir: " + this->__path).c_str()};
  
  if((this->__dir = opendir(std::to_string(this->__path).c_str())) == nullptr)
    throw std::invalid_argument{std::to_string(L"cannot open dir: " + this->__path).c_str()};
}

inline auto DirSelector::format(std::wstring& s) const noexcept -> std::wstring {
  if(!s.empty()) {
    while(s.find(L"./") != std::wstring::npos)
          s.replace(s.find(L"./"), 2, L"");
    s = s.substr(s.find_first_not_of(L'/'));
    if(s.back() != L'/') s += L"/";
  } return s;
}

inline auto DirSelector::operator/(std::wstring&& dir_name) noexcept(false) -> DirSelector {
  return DirSelector{this->__path + this->format(dir_name)};
}
inline auto DirSelector::operator/=(std::wstring&& dir_name) noexcept(false) -> void {
  this->__recover_path = this->__path;
  this->__path += this->format(dir_name); this->update();
}

inline auto DirSelector::operator+(std::wstring&& dir_name) noexcept(false) -> DirSelector {
  return this->operator/(std::move(dir_name));
}
inline auto DirSelector::operator+=(std::wstring&& dir_name) noexcept(false) -> void {
  this->operator/=(std::move(dir_name));
}

inline auto DirSelector::operator-(size_t n) noexcept -> DirSelector {
  if(n >= this->length())
       return DirSelector{L"/"};
  else return DirSelector{this->__path.substr(0,
                          this->find_partition(this->length() - n))};
}
inline auto DirSelector::operator-=(size_t n) noexcept -> void {
  this->__recover_path = this->__path;
  
  if(n >= this->length()) { this->__path = L"/"; return; }
  
  this->__path.replace(this->find_partition
 (this->length() - n), std::wstring::npos, L"");
  this->update();
}
inline auto DirSelector::operator--(void) noexcept -> void {
  this->operator-=(1);
}
inline auto DirSelector::operator--(int) noexcept -> void {
  this->operator-=(1);
}

inline auto DirSelector::operator=(std::wstring&& dir_path) noexcept(false) -> void {
  this->__recover_path = this->__path;
  this->__path = dir_path; this->update();
}

inline auto DirSelector::operator+(void) noexcept -> std::vector<std::wstring> {
  std::vector<std::wstring> r{};
  while((this->__filename = readdir(this->__dir)) != nullptr) {
    if(!strcmp(this->__filename->d_name, ".") ||
       !strcmp(this->__filename->d_name, "..")) continue;
    r.emplace_back(this->__path + std::to_wstring(this->__filename->d_name));
  } return r;
}

inline auto DirSelector::operator-(void) noexcept -> std::vector<std::wstring> {
  std::vector<std::wstring> r{};
  while((this->__filename = readdir(this->__dir)) != nullptr) {
    if(!strcmp(this->__filename->d_name, ".") ||
       !strcmp(this->__filename->d_name, "..")) continue;
    r.emplace_back(std::to_wstring(this->__filename->d_name));
  } return r;
}

inline auto DirSelector::operator&(void) const noexcept -> std::wstring {
  return this->__path;
}

inline auto DirSelector::operator[](size_t v) const noexcept -> std::wstring {
  if(v > this->length()) return L"err";
  
  size_t r{this->find_partition(v)};
  return   this->__path.substr(r,
           this->find_partition(v+1) - r + 1);
}

inline auto DirSelector::operator[](std::pair<size_t, size_t> v) const noexcept -> std::wstring {
  if(v.first >  v.second) std::swap(v.first, v.second);
  if(v.first == v.second) return this->operator[](v.first);
  if(v.second > this->length()) return L"err";
  
  size_t r{this->find_partition(v.first)};
  return   this->__path.substr(r,
   this->find_partition(v.second) - r + 1);
}

inline auto DirSelector::count_file(void) noexcept -> size_t {
  size_t __count{0};
  while((this->__filename = readdir(this->__dir)) != nullptr)
    if (this->__filename->d_type == DT_REG) ++__count;

  return __count;
}


inline auto DirSelector::find_partition(size_t v) const noexcept -> size_t {
  size_t __tmp{0};
  for(; v > 0; --v) if((__tmp = this->__path.find(L"/", __tmp + 1)) == std::wstring::npos) {
    return std::wstring::npos;
  } return __tmp;
}

inline auto DirSelector::length(void) const noexcept -> size_t {
  size_t __tmp{0};
  for(const auto& i : this->__path)
    if(i == L'/') ++__tmp;
  return __tmp - 1;
}

inline auto DirSelector::check(std::wstring&& dir_name) const noexcept -> bool {
  struct stat __tmp_stat{}; lstat(std::to_string(this->__path + this->format(dir_name)).c_str(), &__tmp_stat);
  return S_ISDIR(__tmp_stat.st_mode) && opendir(std::to_string(this->__path + this->format(dir_name)).c_str()) != nullptr;
}

inline auto DirSelector::isPreviousValid(void) noexcept -> bool {
  if(!this->length())
       return false;
  else return opendir(std::to_string(this->__path.substr(0,
                this->find_partition(this->length() - 1))).c_str()) != nullptr;
}

inline auto DirSelector::recover(void) noexcept -> void {
  this->__path.swap(this->__recover_path); this->update();
}
