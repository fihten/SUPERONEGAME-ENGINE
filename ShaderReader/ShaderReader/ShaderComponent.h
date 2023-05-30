#pragma once
#include <string>
#include <list>

class ShaderVisitor;

namespace ShaderUnits
{
	class ShaderComposite;

	class ShaderComponent
	{
		std::string name = "";
		std::list<ShaderComponent*> parents;

	private:
		void addParent(ShaderComponent* parent)
		{
			if (std::find(parents.begin(), parents.end(), parent) == parents.end())
				parents.push_back(parent);
		};
		void removeParent(ShaderComponent* parent)
		{
			if (std::find(parents.begin(), parents.end(), parent) != parents.end())
				parents.remove(parent);
		}
		std::list<ShaderComponent*> getParents() { return parents; };

	private:
		friend class ShaderComposite;

	public:
		virtual ~ShaderComponent()
		{
			auto parents = this->getParents();
			for (auto parent : parents)
				parent->remove(this);
		};
	public:
		virtual void add(ShaderComponent* component) {};
		virtual void remove(ShaderComponent* component) {};
		virtual void query(ShaderVisitor* visitor);
		void setName(const std::string& name) { this->name = name; };
		std::string getName() const { return name; };
	};

	class ShaderComposite : public ShaderComponent
	{
	protected:
		std::list<ShaderComponent*> childs;
	public:
		~ShaderComposite()
		{
			for (auto* child : childs)
			{
				child->removeParent(this);
				delete child;
			}
		};
	public:
		void add(ShaderComponent* component)
		{
			component->addParent(this);
			childs.push_back(component);
		};
		void remove(ShaderComponent* component)
		{
			if (std::find(childs.begin(), childs.end(), component) != childs.end())
			{
				childs.remove(component);
				component->removeParent(this);
			}
		}
		void query(ShaderVisitor* visitor);
	};

	class SHADER : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class Leaf : public ShaderComponent
	{

	};

	class CBUFFER : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SAMPLER_STATE : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class TYPE : public ShaderComposite
	{

	};

	class FLOAT3X3 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLOAT4X4 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class VOID_ : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class BOOL1 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class INT1 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class UINT1 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLOAT1 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLOAT2 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLOAT3 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLOAT4 : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class TRIANGLE_STREAM : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class STRUCT : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class TEXTURE2D : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class TEXTURE2DARRAY : public TYPE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SEMANTIC : public Leaf
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SV_POSITION : public SEMANTIC
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SV_TARGET : public SEMANTIC
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SV_PRIMITIVE_ID : public SEMANTIC
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FUNCTION_DECL : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FUNCTION_CALL : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class MUL : public FUNCTION_CALL
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLOAT2_CONSTRUCTOR : public FUNCTION_CALL
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLOAT3_CONSTRUCTOR : public FUNCTION_CALL
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLOAT4_CONSTRUCTOR : public FUNCTION_CALL
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FUNCTION : public ShaderComposite
	{

	};

	class SET_VERTEX_SHADER : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SET_GEOMETRY_SHADER : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SET_PIXEL_SHADER : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class COMPILE_SHADER : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class VERTEX_SHADER_VERSION : public Leaf
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class PIXEL_SHADER_VERSION : public Leaf
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class MODIFIER : public Leaf
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class IN_MODIFIER : public MODIFIER
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class OUT_MODIFIER : public MODIFIER
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class INOUT_MODIFIER : public MODIFIER
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class UNIFORM : public MODIFIER
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class POINT : public MODIFIER
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class VARIABLE : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class NUMBER : public Leaf
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class COMMENT : public Leaf
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class VARIABLE_DECL : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class BINARY_MINUS : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class BINARY_PLUS : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class BINARY_DIVIDE : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class BINARY_MULTIPLY : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class LESS_THAN : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class GREATER_THAN : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class UNARY_MINUS : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class UNARY_PLUS : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class INCREMENT : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class ASSIGNMENT : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class ADDITION_ASSIGN :public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class DIVIDES_ASSIGN :public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class MULTIPLIES_ASSIGN :public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class TECHNIQUE11 : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class PASS : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class ROUND_BRACKETS : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SQUARE_BRACKETS : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class CURLY_BRACKETS : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class TEMPLATE_PARAMETER :public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class RETURN : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class IF_NODE : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FOR : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class INIT_EXPRESSION : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class COND_EXPRESSION : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class LOOP_EXPRESSION : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class ATTRIBUTE : public Leaf
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class FLATTEN : public ATTRIBUTE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class UNROLL : public ATTRIBUTE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SELECTED_FIELD : public VARIABLE
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class SELECTED_METHOD : public FUNCTION_CALL
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class CAST : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};

	class MAXVERTEXCOUNT : public ShaderComposite
	{
	public:
		void query(ShaderVisitor* visitor);
	};
}