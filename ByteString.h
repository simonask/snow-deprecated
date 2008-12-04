#ifndef BYTESTRING_H_HGUKTH66
#define BYTESTRING_H_HGUKTH66

#include <stdlib.h>

class ByteString {
private:
	unsigned char* m_Data;
	size_t m_Length;
	size_t m_Size;
	
	void enlarge(size_t new_size);
	void enlargeIfNecessary(size_t tryingToAppend);
public:
	class ref {
	private:
		ByteString& str;
		size_t index;
	public:
		ref(ByteString& s, size_t i) : str(s), index(i) {}
		operator unsigned char&() const { return str.get(index); }
		ref& operator=(unsigned char val) { str.set(index, val); }
	};
	
	class const_ref {
	private:
		const ByteString& str;
		size_t index;
	public:
		const_ref(const ByteString& s, size_t i) : str(s), index(i) {}
		operator unsigned char() const { return str.get(index); }
	};
	
	ByteString();
	explicit ByteString(const char* zstr);
	ByteString(const char* str, size_t len);
	ByteString(const ByteString& other);
	ByteString& operator=(const ByteString& other);
	~ByteString();
	
	size_t length() const { return m_Length; }
	size_t size() const { return m_Size; }
	bool empty() const { return m_Length == 0; }
	void copyTo(unsigned char* target) const;
	
	unsigned char& get(size_t index) const;
	unsigned char& set(size_t index, unsigned char value);
	void clear();
	
	void append(char byte);
	void append(unsigned char byte);
	void append(const char* bytes);
	void append(const char* bytes, size_t len);
	void append(const unsigned char* bytes);
	void append(const unsigned char* bytes, size_t len);
	void append(const ByteString& other);
	ByteString& operator<<(char byte);
	ByteString& operator<<(unsigned char byte);
	ByteString& operator<<(const char* bytes);
	ByteString& operator<<(const unsigned char* bytes);
	ByteString& operator<<(const ByteString& other);
	
	ref operator[](size_t index) { return ref(*this, index); }
	const_ref operator[](size_t index) const { return const_ref(*this, index); }
};

#endif /* end of include guard: BYTESTRING_H_HGUKTH66 */
