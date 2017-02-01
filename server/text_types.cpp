/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "text_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>




Suubjects::~Suubjects() throw() {
}


void Suubjects::__set_index(const int32_t val) {
  this->index = val;
__isset.index = true;
}

void Suubjects::__set_text(const std::set<std::string> & val) {
  this->text = val;
__isset.text = true;
}

uint32_t Suubjects::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->index);
          this->__isset.index = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_SET) {
          {
            this->text.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readSetBegin(_etype3, _size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              std::string _elem5;
              xfer += iprot->readString(_elem5);
              this->text.insert(_elem5);
            }
            xfer += iprot->readSetEnd();
          }
          this->__isset.text = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Suubjects::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Suubjects");

  if (this->__isset.index) {
    xfer += oprot->writeFieldBegin("index", ::apache::thrift::protocol::T_I32, 1);
    xfer += oprot->writeI32(this->index);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.text) {
    xfer += oprot->writeFieldBegin("text", ::apache::thrift::protocol::T_SET, 2);
    {
      xfer += oprot->writeSetBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->text.size()));
      std::set<std::string> ::const_iterator _iter6;
      for (_iter6 = this->text.begin(); _iter6 != this->text.end(); ++_iter6)
      {
        xfer += oprot->writeString((*_iter6));
      }
      xfer += oprot->writeSetEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Suubjects &a, Suubjects &b) {
  using ::std::swap;
  swap(a.index, b.index);
  swap(a.text, b.text);
  swap(a.__isset, b.__isset);
}

Suubjects::Suubjects(const Suubjects& other7) {
  index = other7.index;
  text = other7.text;
  __isset = other7.__isset;
}
Suubjects& Suubjects::operator=(const Suubjects& other8) {
  index = other8.index;
  text = other8.text;
  __isset = other8.__isset;
  return *this;
}
void Suubjects::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Suubjects(";
  out << "index="; (__isset.index ? (out << to_string(index)) : (out << "<null>"));
  out << ", " << "text="; (__isset.text ? (out << to_string(text)) : (out << "<null>"));
  out << ")";
}


