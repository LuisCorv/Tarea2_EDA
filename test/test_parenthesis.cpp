#include "adts/node.hpp"
#include "adts/stack.hpp"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace eda;

std::string get_name(std:: string * nombre_de_donde_obtener, std::string end){
	int size_obtener= nombre_de_donde_obtener->length();
	std::string extra;
	std::string copia=*nombre_de_donde_obtener;
	if(end==".html"){
		extra=copia.erase(size_obtener-5)+".log";
	}
	if (end==">"){
		extra="</"+copia.erase(0,1);
	}
	return extra;
}

void get_tags(std::string input, std::string *return_input, int i){
	
	std::string copia=input;
	
	for(int k1=i; k1<input.length();k1++){
		if (input[k1]=='>'){
			copia.erase(k1+1);
			break;
		}
	}
	if (i!=0){
		copia.erase(0,i);
	}
	*return_input=copia;
}

bool validatetags(const std::string &input, int* line, std::string * top_tag, std::string * error_tag, std::fstream * out){
	bool error = false;
	int i = 0;
	Stack stack;
	
	while (!error && i < input.length()){
		std::string input1;
		if (input[i]=='<'){
			get_tags(input,&input1,i);

				//tags de apertura
			if (input1 == "<body>") {
				stack.push(new Node("<body>"));
			}
			if (input1 == "<h1>") {
				stack.push(new Node("<h1>"));
			}
			if (input1 == "<p>") {
				stack.push(new Node("<p>"));
			}
			if (input1 == "<ol>") {
				stack.push(new Node("<ol>"));
			}
			if (input1 == "<li>") {
				stack.push(new Node("<li>"));
			}
			if (input1 == "<center>") {
				stack.push(new Node("<center>"));
			}
			//tags de cierre
			if (input1 == "</body>") {
				if (stack.isEmpty()){
					error = true;
					*error_tag="</body>";
				}
				else{
					if(stack.top()->getData() =="<body>"){
						stack.pop();
						*out<< "tag <body> ok ♪♪ " << std::endl;
					}
					else{
						error = true;
						*error_tag="</body>";
					}
				}
			}
			if (input1 == "</h1>") {
				if (stack.isEmpty()){
					error = true;
					*error_tag="</h1>";
				}
				else{
					if(stack.top()->getData() =="<h1>"){
						stack.pop();
						*out<< "tag <h1> ok ♪♪ " << std::endl;
					}
					else{
						error = true;
						*error_tag="</h1>";
					}
				}
			}
			if (input1 == "</p>") {
				if (stack.isEmpty()){
					error = true;
					*error_tag="</p>";
				}
				else{
					if(stack.top()->getData() =="<p>"){
						stack.pop();
						*out<< "tag <p> ok ♪♪ " << std::endl;
					}
					else{
						error = true;
						*error_tag="</p>";
					}
				}
			}
			if (input1 == "</ol>") {
				if (stack.isEmpty()){
					error = true;
					*error_tag="</ol>";
				}
				else{
					if(stack.top()->getData() =="<ol>"){
						stack.pop();
						*out<< "tag <ol> ok ♪♪ " << std::endl;
					}
					else{
						error = true;
						*error_tag="</ol>";
					}
				}
			}
			if (input1 == "</li>") {
				if (stack.isEmpty()){
					error = true;
					*error_tag="</li>";
				}
				else{
					if(stack.top()->getData() =="<li>"){
						stack.pop();
						*out<< "tag <li> ok ♪♪ " << std::endl;
					}
					else{
						error = true;
						*error_tag="</li>";
					}
				}
			}
			if (input1 == "</center>") {
				if (stack.isEmpty()){
					error = true;
					*error_tag="</center>";
				}
				else{
					if(stack.top()->getData() =="<center>"){
						stack.pop();
						*out<< "tag <center> ok ♪♪ " << std::endl;
					}
					else{
						error = true;
						*error_tag="</center>";
					}
				}
			}
		}
		
		i = i + 1;

	}
	if (!stack.isEmpty() and *error_tag==" "){
		error = true;
		*error_tag=" ninguno";
	}
	*line++;
	*top_tag= stack.top()->getData();
	stack.~Stack();
	return !error;
}



int main(int nargs, char** vargs){
	std::string input;

	int line=0; 	// Nos define la linea del archivo
	std:: string top_tag;	//Nos almacena el top del stack
	std:: string error_tag=" "; //Nos alcena el nombre del tag_que causo error
	std:: string espererado_tag; //Nos almacena el nombre del tag que se deberia haber esperado

	

	bool stat =true;
	
	std::string nombre_archivo=vargs[1];	//ingresar el nombre del archivo .html
	std:: string nombre_salida= get_name(&nombre_archivo,".html");

	std::fstream file;
	std::fstream out;
	file.open(nombre_archivo, std::ios::in);
	
	if (!file){
		std::cout<<"Error, no se puede habrir el archivo"<< nombre_archivo <<std::endl;
	}
	out.open(nombre_salida, std::ios::out);
	if (!out){
		std::cout<<"Error, no se puede habrir el archivo"<< nombre_salida <<std::endl;
	}
	
	
	while( stat){
		std::getline(file, input);
		std::cout<< "output -> "<< input << std::endl;
		stat = validatetags(input, &line, &top_tag, &error_tag, &out); 
		// ♦♦♦♦♦ Arreglar validate (ya que por el no sigue recorriendo el resto de las filas)
		if(file.eof()){
			break;
		}
	}
	
	if (!stat){
		out<< " ---- Error en la linea " ;
		out << line << std::endl;
		
		if (top_tag=="<body>"){
		espererado_tag="</body>";
		}
		if (top_tag=="<h1>"){
			espererado_tag="</h1>";
		}
		if (top_tag=="<p>"){
			espererado_tag="</p>";
		}
		if (top_tag=="<ol>"){
			espererado_tag="</ol>";
		}
		if (top_tag=="<li>"){
			espererado_tag="</li>";
		}
		if (top_tag=="<center>"){
			espererado_tag="</center>";
		}
		if (error_tag=="ninguno"){
			espererado_tag= get_name(&top_tag,">");
		}
		
		out << " Se esperaba" << espererado_tag << "en lugar de" << error_tag << std::endl;
		out << " ----"	<< std::endl;

		std::cout<< "output -> "<< nombre_salida << std::endl;
		return 0;
	}
	
	out<< " 0 Errores ♪♪♪♪" << std::endl;
	
	std::cout<< "output -> "<< nombre_salida << std::endl;
	return 0;
}
