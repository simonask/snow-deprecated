#include "ByteString.h"

#include <string.h>

ByteString::ByteString() : m_Data(NULL), m_Length(0), m_Size(0) {
	
}

ByteString::ByteString(const char* str) : m_Data(NULL), m_Length(0), m_Size(0) {
	append(str);
}

ByteString::ByteString(const char* str, size_t len) : m_Data(NULL), m_Length(0), m_Size(0) {
	append(str, len);
}

ByteString::ByteString(const ByteString& other) : m_Data(NULL), m_Length(0), m_Size(0) {
	append(other);
}

ByteString::~ByteString() {
	free(m_Data);
}

void ByteString::enlarge(size_t new_size) {
	m_Data = (unsigned char*)realloc(m_Data, new_size);
	m_Size = new_size;
}

void ByteString::enlargeIfNecessary(size_t attempt_to_append) {
	if (m_Size - m_Length < attempt_to_append) {
		if (m_Size == 0)
			enlarge(attempt_to_append);
		else
			enlarge(m_Size * 2);
	}
}

ByteString& ByteString::operator=(const ByteString& other) {
	clear();
	enlarge(other.length());
	other.copyTo(m_Data);
	m_Length = other.m_Length;
	return *this;
}

void ByteString::copyTo(unsigned char* buffer) const {
	memcpy(buffer, m_Data, m_Length);
}

unsigned char& ByteString::get(size_t index) const {
	return m_Data[index];
}

unsigned char& ByteString::set(size_t index, unsigned char val) {
	m_Data[index] = val;
	return m_Data[index];
}

void ByteString::clear() {
	memset(m_Data, 0, m_Length);
	m_Length = 0;
}

void ByteString::append(char byte) {
	append((unsigned char) byte);
}

void ByteString::append(unsigned char byte) {
	enlargeIfNecessary(1);
	m_Data[m_Length++] = byte;
}

void ByteString::append(const char* bytes) {
	append(bytes, strlen(bytes));
}

void ByteString::append(const char* bytes, size_t len) {
	append((unsigned char*)bytes, len);
}

void ByteString::append(const unsigned char* bytes) {
	append(bytes, strlen((const char*)bytes));
}

void ByteString::append(const unsigned char* bytes, size_t len) {
	enlargeIfNecessary(len);
	memcpy(&m_Data[m_Length], bytes, len);
	m_Length += len;
}

void ByteString::append(const ByteString& other) {
	append(other.m_Data, other.m_Length);
}

ByteString& ByteString::operator<<(char byte) {
	append(byte);
	return *this;
}

ByteString& ByteString::operator<<(unsigned char byte) {
	append(byte);
	return *this;
}

ByteString& ByteString::operator<<(const char* bytes) {
	append(bytes);
	return *this;
}

ByteString& ByteString::operator<<(const unsigned char* bytes) {
	append(bytes);
	return *this;
}

ByteString& ByteString::operator<<(const ByteString& other) {
	append(other);
	return *this;
}
