#ifndef LABEL_H_KRE0PZ8Q
#define LABEL_H_KRE0PZ8Q

class Label {
private:
	bool m_Bound;
	int m_Offset;
public:
	Label() : m_Bound(false), m_Offset(0) {}
	bool bound() const { return m_Bound; }
	int offset() const { return m_Offset; }
	void bind(int offs) { m_Offset = offs; m_Bound = true; }
	
	bool operator==(const Label& other) const { return m_Bound == other.m_Bound && m_Offset == other.m_Offset; }
};

#endif /* end of include guard: LABEL_H_KRE0PZ8Q */
