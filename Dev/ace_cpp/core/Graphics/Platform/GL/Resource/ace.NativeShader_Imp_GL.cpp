﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.NativeShader_Imp_GL.h"

#include "../ace.Graphics_Imp_GL.h"

#include "../../../../Log/ace.Log.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

#if _WIN32
#define OUTPUT_DEBUG_STRING(s)	OutputDebugStringA(s)
#else
#define OUTPUT_DEBUG_STRING(s)	printf("%s\n",s);
#endif
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp_GL::NativeShader_Imp_GL(
	Graphics* graphics, 
	GLuint program, 
	std::vector<Layout>& layout, 
	int32_t vertexSize,
	std::vector<ConstantLayout>& uniformLayouts,
	int32_t uniformBufferSize,
	std::vector<std::string>& textures)
	: NativeShader_Imp(graphics)
	, m_program(program)
	, m_layout(layout)
	, m_vertexSize(vertexSize)
{
	for (auto& l : uniformLayouts)
	{
		m_constantLayouts[l.Name] = l;
	}
	m_constantBuffer = new uint8_t[uniformBufferSize];

	for (auto i = 0; i < textures.size(); i++)
	{
		if (textures[i] == "") continue;
		m_textureLayouts[textures[i]] = i;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp_GL::~NativeShader_Imp_GL()
{
	glDeleteProgram(m_program);
	SafeDeleteArray(m_constantBuffer);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_GL::Reflect(GLuint program, std::vector<ConstantLayout>& uniformLayouts, int32_t& uniformBufferSize, std::vector<std::string>& textures)
{
	int32_t uniformCount = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);

	uniformLayouts.resize(0);
	int32_t offset = 0;

	for (int32_t u = 0; u < uniformCount; u++)
	{
		char name[256];
		int32_t nameLen = 0;
		GLint size = 0;
		GLenum type;
		glGetActiveUniform(program, u, sizeof(name), &nameLen, &size, &type, name);

		if (type == GL_SAMPLER_2D)
		{
			textures.push_back(name);
		}
		else if (type == GL_SAMPLER_CUBE)
		{
			textures.push_back(name);
		}
		else
		{
			ConstantLayout l;
			l.Name = name;
			l.ID = glGetUniformLocation(program, name);
			l.Offset = offset;
			l.Count = size;

			if (type == GL_FLOAT)
			{
				l.Type = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT1;
				offset += sizeof(float) * 1 * l.Count;
			}
			else if (type == GL_FLOAT_VEC2)
			{
				l.Type = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT2;
				offset += sizeof(float) * 2 * l.Count;
			}
			else if (type == GL_FLOAT_VEC3)
			{
				l.Type = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT3;
				offset += sizeof(float) * 3 * l.Count;
			}
			else if (type == GL_FLOAT_VEC4)
			{
				l.Type = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
				offset += sizeof(float) * 4 * l.Count;
			}
			else if (type == GL_FLOAT_MAT4)
			{
				l.Type = eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_MATRIX44;
				offset += sizeof(float) * 16 * l.Count;
			}
			else
			{
				printf("unknown\n");
				continue;
			}

			uniformLayouts.push_back(l);
		}
	}

	uniformBufferSize = offset;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_GL::CreateVertexConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info)
{
	m_vertexConstantLayouts.clear();

	for (auto& i : info)
	{
		auto id = glGetUniformLocation(m_program, i.Name.c_str());

		ConstantLayout c;
		c.ID = id;
		c.Type = i.Format;
		c.Offset = i.Offset;
		c.Count = i.Count;
		m_vertexConstantLayouts.push_back(c);
	}

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_GL::CreatePixelConstantBufferInternal(int32_t size, std::vector <ConstantBufferInformation>& info)
{
	m_pixelConstantLayouts.clear();

	for (auto& i : info)
	{
		auto id = glGetUniformLocation(m_program, i.Name.c_str());

		ConstantLayout c;
		c.ID = id;
		c.Type = i.Format;
		c.Offset = i.Offset;
		c.Count = i.Count;
		m_pixelConstantLayouts.push_back(c);
	}

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_GL::SetConstantBuffer(const char* name, const void* data, int32_t size)
{
	auto key = std::string(name);

	auto it = m_constantLayouts.find(key);

	if (it != m_constantLayouts.end())
	{
		auto size_ = GetBufferSize(it->second.Type, it->second.Count);
		assert(size == size_);

		memcpy(&(m_constantBuffer[it->second.Offset]), data, size);
	}
}

void NativeShader_Imp_GL::SetTexture(const char* name, Texture* texture, TextureFilterType filterType, TextureWrapType wrapType)
{
	auto key = std::string(name);
	auto g = (Graphics_Imp_GL*) GetGraphics();

	auto it = m_textureLayouts.find(key);

	if (it != m_textureLayouts.end())
	{
		NativeShader_Imp::SetTexture(name, texture, filterType, wrapType, (*it).second);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_GL::AssignConstantBuffer()
{
	for (auto l_ = m_constantLayouts.begin(); l_ != m_constantLayouts.end(); l_++)
	{
		auto& l = l_->second;

		if (l.Type == CONSTANT_BUFFER_FORMAT_MATRIX44)
		{
			uint8_t* data = (uint8_t*) m_constantBuffer;
			data += l.Offset;
			glUniformMatrix4fv(
				l.ID,
				1,
				GL_TRUE,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY)
		{
			uint8_t* data = (uint8_t*) m_constantBuffer;
			data += l.Offset;
			glUniformMatrix4fv(
				l.ID,
				l.Count,
				GL_TRUE,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT4)
		{
			uint8_t* data = (uint8_t*) m_constantBuffer;
			data += l.Offset;
			glUniform4fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT1)
		{
			uint8_t* data = (uint8_t*) m_constantBuffer;
			data += l.Offset;
			glUniform1fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT2)
		{
			uint8_t* data = (uint8_t*) m_constantBuffer;
			data += l.Offset;
			glUniform2fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT3)
		{
			uint8_t* data = (uint8_t*) m_constantBuffer;
			data += l.Offset;
			glUniform3fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
	}

	for (auto& l : m_vertexConstantLayouts)
	{
		if (l.Type == CONSTANT_BUFFER_FORMAT_MATRIX44)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += l.Offset;
			glUniformMatrix4fv(
				l.ID,
				1,
				GL_TRUE,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += l.Offset;
			glUniformMatrix4fv(
				l.ID,
				l.Count,
				GL_TRUE,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT4)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += l.Offset;
			glUniform4fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT1)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += l.Offset;
			glUniform1fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT2)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += l.Offset;
			glUniform2fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT3)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += l.Offset;
			glUniform3fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
	}

	for (auto& l : m_pixelConstantLayouts)
	{
		if (l.Type == CONSTANT_BUFFER_FORMAT_MATRIX44)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += l.Offset;
			glUniformMatrix4fv(
				l.ID,
				1,
				GL_TRUE,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += l.Offset;
			glUniformMatrix4fv(
				l.ID,
				l.Count,
				GL_TRUE,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT4)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += l.Offset;
			glUniform4fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT1)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += l.Offset;
			glUniform1fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT2)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += l.Offset;
			glUniform2fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
		else if (l.Type == CONSTANT_BUFFER_FORMAT_FLOAT3)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += l.Offset;
			glUniform3fv(
				l.ID,
				1,
				(const GLfloat*) data);
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp_GL* NativeShader_Imp_GL::Create(
	Graphics* graphics,
	const char* vertexShaderText,
	const char* vertexShaderFileName,
	const char* pixelShaderText,
	const char* pixelShaderFileName,
	std::vector <VertexLayout>& layout,
	std::vector <Macro>& macro,
	Log* log)
{
	GLCheckError();

	char* vs_src[3];
	char* ps_src[3];
	int32_t vs_src_len[3];
	int32_t ps_src_len[3];

	int32_t vs_src_count = 0;
	int32_t ps_src_count = 0;

	GLuint program, vs_shader, ps_shader;
	GLint res_vs, res_ps, res_link;

	std::string macros;

	if (macro.size() > 0)
	{
		for ( auto& m : macro)
		{
			macros += (std::string("#define ") + std::string(m.Name) + std::string(" ") + std::string(m.Definition) + std::string("\n"));
		}

		vs_src[0] = (char*)macros.c_str();
		vs_src_len[0] = strlen(vs_src[0]);

		ps_src[0] = (char*) macros.c_str();
		ps_src_len[0] = strlen(ps_src[0]);

		vs_src[1] = (char*) vertexShaderText;
		vs_src_len[1] = strlen(vs_src[1]);
		
		ps_src[1] = (char*) pixelShaderText;
		ps_src_len[1] = strlen(ps_src[1]);
		
		vs_src_count = 2;
		ps_src_count = 2;
	}
	else
	{
		vs_src[0] = (char*)vertexShaderText;
		vs_src_len[0] = strlen(vs_src[0]);
		
		ps_src[0] = (char*) pixelShaderText;
		ps_src_len[0] = strlen(ps_src[0]);

		vs_src_count = 1;
		ps_src_count = 1;
	}

	/* 頂点シェーダーコンパイル */
	vs_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_shader, vs_src_count, (const GLchar**) vs_src, vs_src_len);
	glCompileShader(vs_shader);
	glGetShaderiv(vs_shader, GL_COMPILE_STATUS, &res_vs);

	/* ピクセル(フラグメントシェーダー)コンパイル */
	ps_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ps_shader, ps_src_count, (const GLchar**) ps_src, ps_src_len);
	glCompileShader(ps_shader);
	glGetShaderiv(ps_shader, GL_COMPILE_STATUS, &res_ps);

	/* プログラム生成 */
	program = glCreateProgram();
	glAttachShader(program, ps_shader);
	glAttachShader(program, vs_shader);

	/* リンク */
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &res_link);

	if (
		res_vs == GL_FALSE ||
		res_ps == GL_FALSE ||
		res_link == GL_FALSE)
	{

		log->WriteHeading("シェーダー生成失敗");
		log->WriteLine(vertexShaderFileName);
		log->WriteLine(pixelShaderFileName);

		char log_text[512];
		int32_t log_size;
		glGetShaderInfoLog(vs_shader, sizeof(log_text), &log_size, log_text);
		if (log_size > 0)
		{
			OUTPUT_DEBUG_STRING("Vertex Shader:\n");
			OUTPUT_DEBUG_STRING(vertexShaderFileName);
			OUTPUT_DEBUG_STRING("\n");
			OUTPUT_DEBUG_STRING(log_text);
			log->WriteLine(log_text);
		}
		glGetShaderInfoLog(ps_shader, sizeof(log_text), &log_size, log_text);
		if (log_size > 0)
		{
			OUTPUT_DEBUG_STRING("Fragment Shader:\n");
			OUTPUT_DEBUG_STRING(pixelShaderFileName);
			OUTPUT_DEBUG_STRING("\n");
			OUTPUT_DEBUG_STRING(log_text);
			log->WriteLine(log_text);
		}
		glGetProgramInfoLog(program, sizeof(log_text), &log_size, log_text);
		if (log_size > 0)
		{
			OUTPUT_DEBUG_STRING("Shader Link:\n");
			OUTPUT_DEBUG_STRING(vertexShaderFileName);
			OUTPUT_DEBUG_STRING("\n");
			OUTPUT_DEBUG_STRING(pixelShaderFileName);
			OUTPUT_DEBUG_STRING("\n");
			OUTPUT_DEBUG_STRING(log_text);
			log->WriteLine(log_text);
		}

		glDeleteProgram(program);
		glDeleteShader(vs_shader);
		glDeleteShader(ps_shader);

		return nullptr;
	}

	{
		char vs_text[512];
		int32_t vs_size;

		char ps_text[512];
		int32_t ps_size;

		char l_text[512];
		int32_t l_size;

		glGetShaderInfoLog(vs_shader, sizeof(vs_text), &vs_size, vs_text);
		glGetShaderInfoLog(ps_shader, sizeof(ps_text), &ps_size, ps_text);
		glGetProgramInfoLog(program, sizeof(l_text), &l_size, l_text);

		if (vs_size > 0 || ps_size > 0 || l_size > 0)
		{
			log->WriteHeading("シェーダー生成中");
			log->WriteLine(vertexShaderFileName);
			log->WriteLine(pixelShaderFileName);

			if (vs_size > 0)
			{
				OUTPUT_DEBUG_STRING("Vertex Shader:\n");
				OUTPUT_DEBUG_STRING(vertexShaderFileName);
				OUTPUT_DEBUG_STRING("\n");
				OUTPUT_DEBUG_STRING(vs_text);
				log->WriteLine(vs_text);
			}

			if (ps_size > 0)
			{
				OUTPUT_DEBUG_STRING("Fragment Shader:\n");
				OUTPUT_DEBUG_STRING(pixelShaderFileName);
				OUTPUT_DEBUG_STRING("\n");
				OUTPUT_DEBUG_STRING(ps_text);
				log->WriteLine(ps_text);
			}

			if (l_size > 0)
			{
				OUTPUT_DEBUG_STRING("Shader Link:\n");
				OUTPUT_DEBUG_STRING(vertexShaderFileName);
				OUTPUT_DEBUG_STRING("\n");
				OUTPUT_DEBUG_STRING(pixelShaderFileName);
				OUTPUT_DEBUG_STRING("\n");
				OUTPUT_DEBUG_STRING(l_text);
				log->WriteLine(l_text);
			}
		}
	}

	glDeleteShader(vs_shader);
	glDeleteShader(ps_shader);
	GLCheckError();

	std::vector<ConstantLayout> uniformLayouts;
	int32_t uniformBufferSize = 0;
	std::vector<std::string> textures;
	Reflect(program, uniformLayouts, uniformBufferSize, textures);
	GLCheckError();

	std::vector<Layout> layout_;
	uint16_t byteOffset = 0;

	for (auto& l : layout)
	{
		auto att = glGetAttribLocation(program, l.Name.c_str());
		Layout l_;
		l_.attribute = att;
	
		if (l.LayoutFormat == LAYOUT_FORMAT_R32G32B32_FLOAT)
		{
			l_.type = GL_FLOAT;
			l_.count = 3;
			l_.normalized = false;
			l_.offset = byteOffset;
			byteOffset += sizeof(float) * 3;
		}
		else if (l.LayoutFormat == LAYOUT_FORMAT_R8G8B8A8_UNORM)
		{
			l_.type = GL_UNSIGNED_BYTE;
			l_.count = 4;
			l_.normalized = true;
			l_.offset = byteOffset;
			byteOffset += sizeof(float) * 1;
		}
		else if (l.LayoutFormat == LAYOUT_FORMAT_R8G8B8A8_UINT)
		{
			l_.type = GL_UNSIGNED_BYTE;
			l_.count = 4;
			l_.normalized = false;
			l_.offset = byteOffset;
			byteOffset += sizeof(float) * 1;
		}
		else if (l.LayoutFormat == LAYOUT_FORMAT_R32G32_FLOAT)
		{
			l_.type = GL_FLOAT;
			l_.count = 2;
			l_.normalized = false;
			l_.offset = byteOffset;
			byteOffset += sizeof(float) * 2;
		}

		layout_.push_back(l_);
	}

	GLCheckError();

	return new NativeShader_Imp_GL(
		graphics, 
		program, 
		layout_, 
		byteOffset,
		uniformLayouts,
		uniformBufferSize,
		textures);
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_GL::SetLayout()
{
	const void* vertices = NULL;

	for (size_t i = 0; i < m_layout.size(); i++)
	{
		if (m_layout[i].attribute >= 0)
		{
			auto& layout = m_layout[i];

			glEnableVertexAttribArray(m_layout[i].attribute);
			glVertexAttribPointer(layout.attribute, layout.count, layout.type, layout.normalized, m_vertexSize, (uint8_t*) vertices + layout.offset);
		}
	}

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void NativeShader_Imp_GL::Disable()
{
	for (size_t i = 0; i < m_layout.size(); i++)
	{
		if (m_layout[i].attribute >= 0)
		{
			auto& layout = m_layout[i];

			glDisableVertexAttribArray(m_layout[i].attribute);
		}
	}

	glUseProgram(0);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
