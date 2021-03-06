#include <GL/glew.h>
#include <GL/gl.h>

#include "glstates.h"



glstate_t state;

void states_blendFunc(const GLenum source, const GLenum dest){
	if(source == state.blendsource && dest == state.blenddest)return;
	glBlendFunc(source, dest);
	state.blendsource = source;
	state.blenddest = dest;
}
void states_alpha(const GLenum func, const GLclampf ref){
	if(func == state.alphafunc && ref == state.alpharef)return;
	glAlphaFunc(func, ref);
	state.alphafunc = func;
	state.alpharef = ref;
}
void states_depthFunc(const GLenum dfunc){
	if(dfunc == state.depthfunc) return;
	glDepthFunc(dfunc);
	state.depthfunc = dfunc;
}
void states_cullFace(const GLenum face){
	if(face == state.cullface) return;
	glCullFace(face);
	state.cullface = face;
}
void states_depthMask(const GLboolean mask){
	if(mask == state.depthmask) return;
	glDepthMask(mask);
	state.depthmask = mask;
}
void states_bindVertexArray(const GLuint id){
	if(id != state.vaoid){
		glBindVertexArray(id);
		state.vaoid = id;
	}
}
void states_bindBuffer(const GLenum type, const GLuint id){
	if(type != state.vbotype || state.vborangei ||id != state.vboid || state.vborangeo || state.vboranges){
		glBindBuffer(type, id);
		state.vbotype = type;
		state.vborangei = 0;
		state.vboid = id;
		state.vborangeo = 0;
		state.vboranges = 0;
	}
}
void states_bindBufferRange(const GLenum type, const GLuint index, const GLuint id, const GLintptr offset, const GLsizeiptr size){
	//todo redo to correctly handle index
	if(type != state.vbotype || index != state.vborangei||id != state.vboid || offset != state.vborangeo || size != state.vboranges){
		glBindBufferRange(type, index, id, offset, size);
		state.vbotype = type;
		state.vborangei = index;
		state.vboid = id;
		state.vborangeo = offset;
		state.vboranges = size;
	}
}
char states_useProgram(const GLuint shaderid){
	if(shaderid != state.shaderid){
		glUseProgram(shaderid);
		state.shaderid = shaderid;
		return 2;
	}
	return 1;
}
/*
void states_activeTexture(const GLenum activetexture){
	if(activetexture != state.activetexture){
		glActiveTexture(activetexture);
		state.activetexture = activetexture;
	}
}
*/
void states_activeTexture(const unsigned char activetexture){
	if(activetexture > STATESTEXTUREUNITCOUNT) return; //todo error return?
	if(activetexture != state.activetexture){
		glActiveTexture(GL_TEXTURE0 + activetexture);
		state.activetexture = activetexture;
	}
}
void states_bindTexture(const GLenum target, const GLuint id){
	if(id != state.textureunitid[state.activetexture] ||target != state.textureunittarget[state.activetexture]){
		glBindTexture(target, id);
		state.textureunitid[state.activetexture] = id;
		state.textureunittarget[state.activetexture] = target;
	}
}
void states_bindActiveTexture(const unsigned char activetexture, const GLenum target, const GLuint id){
	if(id != state.textureunitid[activetexture] ||target != state.textureunittarget[activetexture]){
		if(activetexture != state.activetexture){
			glActiveTexture(GL_TEXTURE0 + activetexture);
			state.activetexture = activetexture;
		}
		glBindTexture(target, id);
		state.textureunitid[activetexture] = id;
		state.textureunittarget[activetexture] = target;
	}
}
void states_bindTextures(const GLuint first, const GLsizei count, const GLuint * textures){
	//TODO REDO
	GLsizei fixedcount = (first + count > STATESTEXTUREUNITCOUNT)? STATESTEXTUREUNITCOUNT - first : count;
//	GLsizei mycount;
//	unsigned int myfirst = first;
//	unsigned int myend = first + mycount;
	//compress front
	//TODO compress back too
	unsigned int i;
//	unsigned int k;
	for(i = 0; i < fixedcount; i++){
		if(textures[i] && state.textureunitid[first + i] != textures[i]) break;
	}
	//didnt make it all the way to the end.. need to update some texture binds
	if(i != fixedcount){
//		myfirst = first + i;
//		mycount = fixedcount - i;
		glBindTextures(first + i, fixedcount - i, textures + i);
		//update state
		for(; i < fixedcount; i++){
			state.textureunitid[first + i] = textures[i];
		}
	}
}


void states_enableForce(const GLenum en){
	switch(en){
		case GL_DEPTH_TEST:
			state.enabledstates = state.enabledstates | STATESENABLEDEPTH;
		break;
		case GL_BLEND:
			state.enabledstates = state.enabledstates | STATESENABLEBLEND;
		break;
		case GL_CULL_FACE:
			state.enabledstates = state.enabledstates | STATESENABLECULLFACE;
		break;
		case GL_MULTISAMPLE:
			state.enabledstates = state.enabledstates | STATESENABLEMULTISAMPLE;
		break;
		case GL_ALPHA_TEST:
			state.enabledstates = state.enabledstates | STATESENABLEALPHATEST;
		break;
		default:
		break;
	}
	glEnable(en);
}
void states_setState(const glstate_t s){
	if(s.enabledstates != state.enabledstates){
		char teststates = s.enabledstates ^ state.enabledstates;
		if(teststates & STATESENABLEDEPTH) (s.enabledstates & STATESENABLEDEPTH) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		if(teststates & STATESENABLEBLEND) (s.enabledstates & STATESENABLEBLEND) ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		if(teststates & STATESENABLECULLFACE) (s.enabledstates & STATESENABLECULLFACE) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		if(teststates & STATESENABLEMULTISAMPLE) (s.enabledstates & STATESENABLEMULTISAMPLE) ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
		if(teststates & STATESENABLEALPHATEST) (s.enabledstates & STATESENABLEALPHATEST) ? glEnable(GL_ALPHA_TEST) : glDisable(GL_ALPHA_TEST);
		//todo add more
		state.enabledstates = s.enabledstates;
	}
	if((s.enabledstates & STATESENABLEBLEND) && (s.blendsource != state.blendsource || s.blenddest != state.blenddest)){
		glBlendFunc(s.blendsource, s.blenddest);
		state.blendsource = s.blendsource;
		state.blenddest = s.blenddest;
	}
	if((s.enabledstates & STATESENABLEALPHATEST) && (s.alphafunc != state.alphafunc || s.alpharef != state.alpharef)){
		glAlphaFunc(s.alphafunc, s.alpharef);
		state.alphafunc = s.alphafunc;
		state.alpharef = s.alpharef;
	}
	if((s.enabledstates & STATESENABLEDEPTH) && (s.depthfunc != state.depthfunc) ){
		glDepthFunc(s.depthfunc);
		state.depthfunc = s.depthfunc;
	}
	if((s.enabledstates & STATESENABLECULLFACE) && (s.cullface != state.cullface) ){
		glCullFace(s.cullface);
		state.cullface = s.cullface;
	}
	if(s.depthmask != state.depthmask){
		glDepthMask(s.depthmask);
		state.depthmask = s.depthmask;
	}
	if(s.vaoid != state.vaoid){
		glBindVertexArray(s.vaoid);
		state.vaoid = s.vaoid;
	}
	if(s.shaderid != state.shaderid){
		glUseProgram(s.shaderid);
		state.shaderid = s.shaderid;
	}
/*
	if(s.vboranges){
		if(s.vbotype != state.vbotype || s.vborangei != state.vborangei||s.vboid != state.vboid || s.vborangeo != state.vborangeo || s.vboranges != state.vboranges){
			glBindBufferRange(s.vbotype, s.vborangei, s.vboid, s.vborangeo, s.vboranges);
			state.vbotype = s.vbotype;
			state.vborangei = s.vborangei;
			state.vboid = s.vboid;
			state.vborangeo = s.vborangeo;
			state.vboranges = s.vboranges;
		}
	} else {
		if(s.vbotype != state.vbotype || state.vborangei ||s.vboid != state.vboid || state.vborangeo || state.vboranges){
			glBindBuffer(s.vbotype, s.vboid);
			state.vbotype = s.vbotype;
			state.vborangei = 0;
			state.vboid = s.vboid;
			state.vborangeo = 0;
			state.vboranges = 0;
		}
	}
*/

//#define CANBINDTEXTURES
//this does not have a noticable improvement in performance on my laptop (hd 6400 something)
	unsigned int i;
#ifdef CANBINDTEXTURES
	//compress front
	for( i = 0; i < STATESTEXTUREUNITCOUNT; i++){
		GLuint id = s.textureunitid[i];
		if(id && id != state.textureunitid[i]) break;
	}
	unsigned int k;
	//compress back
	for( k = STATESTEXTUREUNITCOUNT; k > i; k--){
		GLuint id = s.textureunitid[k-1];
		if(id && id != state.textureunitid[k-1]) break;
	}
	if(k > i){
		//update state
		unsigned int j = i;
		for(; i < k; i++){
			GLuint id = s.textureunitid[i];
			if(id){
				state.textureunitid[i] = id;
				state.textureunittarget[i] = s.textureunittarget[i]; //TODO do i need this?
			}

		}
		//bind
		glBindTextures(j, k-j, &state.textureunitid[j]);
	}
#else
	for(i = 0; i < STATESTEXTUREUNITCOUNT; i++){
		GLuint id = s.textureunitid[i];
		if(id){
			GLenum target = s.textureunittarget[i];
			if(id != state.textureunitid[i] || target != state.textureunittarget[i]){
				//if(i != state.activetexture){ // after the first one in this loop it will always hit, so dont bother checking
					glActiveTexture(GL_TEXTURE0 + i);
					state.activetexture = i;
				//}
				glBindTexture(target, id);
				state.textureunitid[i] = id;
				state.textureunittarget[i] = target;
			}
		}
	}
#endif
	for(i = 0; i < STATESUBOBLOCKCOUNT; i++){
		GLuint id =s.uboblockid[i];
		if(id){ //doesnt matter if the id is 0... may change
			GLuint rangeo = s.uboblockrangeo[i];
			GLuint ranges = s.uboblockranges[i];
			if(id != state.uboblockid[i] || ranges != state.uboblockrangeo[i] || rangeo != state.uboblockranges[i]){
				state.uboblockid[i] = id;
				if(ranges){
					state.uboblockrangeo[i] = rangeo;
					state.uboblockranges[i] = ranges;
					glBindBufferRange(GL_UNIFORM_BUFFER, i, id, rangeo, ranges);
				} else {
					state.uboblockrangeo[i] = 0;
					state.uboblockranges[i] = 0;
					glBindBufferBase(GL_UNIFORM_BUFFER, i, id);
				}
			}
		}
	}
	//todo set up callbacks for if each thing changes... useful for shaders, viewports/framebuffers, etc
}



void states_forceState(const glstate_t s){
	(s.enabledstates & STATESENABLEDEPTH) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	(s.enabledstates & STATESENABLEBLEND) ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	(s.enabledstates & STATESENABLECULLFACE) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	(s.enabledstates & STATESENABLEMULTISAMPLE) ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
	(s.enabledstates & STATESENABLEALPHATEST) ? glEnable(GL_ALPHA_TEST) : glDisable(GL_ALPHA_TEST);
		//todo add more
	state.enabledstates = s.enabledstates;
	glBlendFunc(s.blendsource, s.blenddest);
	state.blendsource = s.blendsource;
	state.blenddest = s.blenddest;

	glAlphaFunc(s.alphafunc, s.alpharef);
	state.alphafunc = s.alphafunc;
	state.alpharef = s.alpharef;

	glDepthFunc(s.depthfunc);
	state.depthfunc = s.depthfunc;

	glCullFace(s.cullface);
	state.cullface = s.cullface;

	glDepthMask(s.depthmask);
	state.depthmask = s.depthmask;

	glBindVertexArray(s.vaoid);
	state.vaoid = s.vaoid;

	glUseProgram(s.shaderid);
	state.shaderid = s.shaderid;
/*
	if(s.vboranges){
		glBindBufferRange(s.vbotype, s.vborangei, s.vboid, s.vborangeo, s.vboranges);
		state.vbotype = s.vbotype;
		state.vborangei = s.vborangei;
		state.vboid = s.vboid;
		state.vborangeo = s.vborangeo;
		state.vboranges = s.vboranges;
	} else {
		glBindBuffer(s.vbotype, s.vboid);
		state.vbotype = s.vbotype;
		state.vborangei = 0;
		state.vboid = s.vboid;
		state.vborangeo = 0;
		state.vboranges = 0;
	}
*/
	unsigned int i;
	for(i = 0; i < STATESTEXTUREUNITCOUNT; i++){
		GLuint id = s.textureunitid[i];
		if(id){
			GLenum target = s.textureunittarget[i];
			//if(i != state.activetexture){
				glActiveTexture(GL_TEXTURE0 + i);
				state.activetexture = i;
			//}
			glBindTexture(target, id);
			state.textureunitid[i] = id;
			state.textureunittarget[i] = target;
		}
	}
	for(i = 0; i < STATESUBOBLOCKCOUNT; i++){
		GLuint id =s.uboblockid[i];
		GLuint rangeo = s.uboblockrangeo[i];
		GLuint ranges = s.uboblockranges[i];
		state.uboblockid[i] = id;
		if(ranges){
			state.uboblockrangeo[i] = rangeo;
			state.uboblockranges[i] = ranges;
			glBindBufferRange(GL_UNIFORM_BUFFER, i, id, rangeo, ranges);
		} else {
			state.uboblockrangeo[i] = 0;
			state.uboblockranges[i] = 0;
			glBindBufferBase(GL_UNIFORM_BUFFER, i, id);
		}
	}
	//todo set up callbacks for if each thing changes... useful for shaders, viewports/framebuffers, etc
}


void states_enable(const GLenum en){
	switch(en){
		case GL_DEPTH_TEST:
			if(!(state.enabledstates & STATESENABLEDEPTH)){
				state.enabledstates = state.enabledstates | STATESENABLEDEPTH;
				glEnable(en);
			}
		break;
		case GL_BLEND:
			if(!(state.enabledstates & STATESENABLEBLEND)){
				state.enabledstates = state.enabledstates | STATESENABLEBLEND;
				glEnable(en);
			}
		break;
		case GL_CULL_FACE:
			if(!(state.enabledstates & STATESENABLECULLFACE)){
				state.enabledstates = state.enabledstates | STATESENABLECULLFACE;
				glEnable(en);
			}
		break;
		case GL_MULTISAMPLE:
			if(!(state.enabledstates & STATESENABLEMULTISAMPLE)){
				state.enabledstates = state.enabledstates | STATESENABLEMULTISAMPLE;
				glEnable(en);
			}
		break;
		case GL_ALPHA_TEST:
			if(!(state.enabledstates & STATESENABLEALPHATEST)){
				state.enabledstates = state.enabledstates | STATESENABLEALPHATEST;
				glEnable(en);
			}
		break;
		default:
			glEnable(en);
		break;
	}
}


void states_disableForce(const GLenum en){
	switch(en){
		case GL_DEPTH_TEST:
			state.enabledstates = state.enabledstates & (~STATESENABLEDEPTH);
		break;
		case GL_BLEND:
			state.enabledstates = state.enabledstates & (~STATESENABLEBLEND);
		break;
		case GL_CULL_FACE:
			state.enabledstates = state.enabledstates & (~STATESENABLECULLFACE);
		break;
		case GL_MULTISAMPLE:
			state.enabledstates = state.enabledstates & (~STATESENABLEMULTISAMPLE);
		break;
		case GL_ALPHA_TEST:
			state.enabledstates = state.enabledstates & (~STATESENABLEALPHATEST);
		break;
		default:
		break;
	}
	glDisable(en);
}

void states_disable(const GLenum en){
	switch(en){
		case GL_DEPTH_TEST:
			if(state.enabledstates & STATESENABLEDEPTH){
				state.enabledstates = state.enabledstates & (~STATESENABLEDEPTH); // better way of doing this?
				glDisable(en);
			}
		break;
		case GL_BLEND:
			if(state.enabledstates & STATESENABLEBLEND){
				state.enabledstates = state.enabledstates & (~STATESENABLEBLEND); // better way of doing this?
				glDisable(en);
			}
		break;
		case GL_CULL_FACE:
			if(state.enabledstates & STATESENABLECULLFACE){
				state.enabledstates = state.enabledstates & (~STATESENABLECULLFACE); // better way of doing this?
				glDisable(en);
			}
		break;
		case GL_MULTISAMPLE:
			if(state.enabledstates & STATESENABLEMULTISAMPLE){
				state.enabledstates = state.enabledstates & (~STATESENABLEMULTISAMPLE); // better way of doing this?
				glDisable(en);
			}
		break;
		case GL_ALPHA_TEST:
			if(state.enabledstates & STATESENABLEALPHATEST){
				state.enabledstates = state.enabledstates & (~STATESENABLEALPHATEST); // better way of doing this?
				glDisable(en);
			}
		break;
		default:
			glDisable(en);
		break;
	}
}
