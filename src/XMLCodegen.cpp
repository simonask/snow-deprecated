#include "XMLCodegen.h"
#include "ASTNode.h"
#include <sstream>

namespace snow {
	std::string XMLCodegen::indent(int level) {
		level += m_BaseIndent;
		std::stringstream ss;
		while (level) {
			ss << ' ';
			level--;
		}
		return ss.str();
	}
	
	
	Handle<CompiledCode> XMLCodegen::compile() {
		compile(m_Def);
		return NULL;
	}
	
	void XMLCodegen::compile(ast::Literal& ltrl) {
		m_Out << "<literal>" << ltrl.string << "</literal>";
	}
	
	void XMLCodegen::compile(ast::Identifier& idt) {
		m_Out << "<identifier>" << idt.name << "</identifier>";
	}
	
	void XMLCodegen::compile(ast::Sequence& sqnc) {
		m_Out << "<sequence>" << std::endl;
		for (auto iter = sqnc.nodes.begin(); iter != sqnc.nodes.end(); iter++) {
			(*iter)->compile(*this);
			m_Out << std::endl;
		}
		m_Out << "</sequence>" << std::endl;
	}
	
	void XMLCodegen::compile(ast::FunctionDefinition& def) {
		m_Out << "<function-definition>" << std::endl;
		for (auto iter = def.arguments.begin(); iter != def.arguments.end(); iter++) {
			m_Out << "<arguments>" << std::endl;
			(*iter)->compile(*this);
			m_Out << std::endl;
			m_Out << "</arguments>" << std::endl;
		}
		def.sequence->compile(*this);
		m_Out << std::endl;
		m_Out << "</function-definition>" << std::endl;
	}
	
	void XMLCodegen::compile(ast::Return& rtrn) {
		if (rtrn.expression != NULL) {
			m_Out << "<return>";
			rtrn.expression->compile(*this);
			m_Out << "</return>" << std::endl;
		} else {
			m_Out << "<return/>" << std::endl;
		}
	}
	
	void XMLCodegen::compile(ast::Throw& thrw) {
		m_Out << "<throw>";
		thrw.object->compile(*this);
		m_Out << "</throw>" << std::endl;
	}
	
	void XMLCodegen::compile(ast::Break& brk) {
		m_Out << "<break/>" << std::endl;
	}
	
	void XMLCodegen::compile(ast::Continue& cnt) {
		m_Out << "<continue/>" << std::endl;
	}
	
	void XMLCodegen::compile(ast::Assignment& asmt) {
		m_Out << "<assignment>" << std::endl;
		asmt.identifier->compile(*this);
		asmt.expression->compile(*this);
		m_Out << "</assignment>" << std::endl;
	}
	
	void XMLCodegen::compile(ast::IfCondition& cnd) {} //FIXME
	void XMLCodegen::compile(ast::IfElseCondition& cnd) {} //FIXME
	
	void XMLCodegen::compile(ast::Call& call) {
		m_Out << "<call>" << std::endl;
		m_Out << "<self>";
		call.self->compile(*this);
		m_Out << "</self>" << std::endl;
		
		if (call.member != NULL)
			m_Out << "<member>";
			call.member->compile(*this);
			m_Out << "</member>" << std::endl;
		
		for (auto iter = call.arguments->nodes.begin(); iter != call.arguments->nodes.end(); iter++) {
			m_Out << "<arguments>" << std::endl;
			(*iter)->compile(*this);
			m_Out << std::endl;
			m_Out << "</arguments>" << std::endl;
		}
		
		m_Out << "</call>" << std::endl;		
	}
	
	void XMLCodegen::compile(ast::Get& get) {
		m_Out << "<get>" << std::endl;
		
		m_Out << "<self>";
		get.self->compile(*this);
		m_Out << "</self>" << std::endl;
		
		m_Out << "<member>";
		get.member->compile(*this);
		m_Out << "</member>" << std::endl;
		
		
		m_Out << "</get>" << std::endl;
	}
	
	void XMLCodegen::compile(ast::Set& set) {
		m_Out << "<set>" << std::endl;
		
		m_Out << "<self>";
		set.self->compile(*this); 
		m_Out << "</self>" << std::endl;
		
		m_Out << "<member>";
		set.member->compile(*this);
		m_Out << "</member>" << std::endl;
		
		set.member->compile(*this);
		
		m_Out << "</set>" << std::endl;
	}
	
	void XMLCodegen::compile(ast::Loop& loop) {
		m_Out << "<loop>" << std::endl;
		
		m_Out << "<expression>" << std::endl;
		loop.expression->compile(*this);
		m_Out << "</expression>" << std::endl;
		
		m_Out << "<while-true>" << std::endl;
		loop.while_true->compile(*this);
		m_Out << "</while-true>" << std::endl;
		
		m_Out << "</loop>" << std::endl;
	}
}