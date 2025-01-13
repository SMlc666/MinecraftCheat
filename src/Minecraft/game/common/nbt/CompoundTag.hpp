#pragma once
#include <map>
#include <string_view>
#include "tag.hpp"

class CompoundTagVariant;
class IntTag;
class StringTag;
class ListTag;
class ByteTag;

typedef std::map<std::string, CompoundTagVariant, std::less<>> TagMap;

class CompoundTag : public Tag {
public:
  TagMap mTags;

  const Tag *get(std::string_view name) const;
  Tag *get(std::string_view);

  bool contains(std::string_view name) const;
  bool contains(std::string_view name, Tag::Type type) const;

  void clear();
  void deepCopy(const CompoundTag &other);
  std::unique_ptr<CompoundTag> clone() const;

  const ListTag *getList(std::string_view name) const;
  ListTag *getList(std::string_view name);

  const ByteTag *getByteTag(std::string_view name) const;
  ByteTag *getByteTag(std::string_view name);
  uint8_t getByte(std::string_view) const;
  void putByte(std::string name, unsigned char value);

  const StringTag *getStringTag(std::string_view name) const;
  StringTag *getStringTag(std::string_view name);
  const std::string *getString(std::string_view name) const;
  void putString(std::string name, std::string value);

  const IntTag *getIntTag(std::string_view name) const;
  IntTag *getIntTag(std::string_view name);
  int getInt(std::string_view name) const;

private:
  template <typename T> const T *get(std::string_view) const;

  template <typename T> T *get(std::string_view);
};