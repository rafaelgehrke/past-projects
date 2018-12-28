#include<iostream>
#include<cstdlib>
#include<string.h>
using namespace std;

void convert(char convert[50]);

class Cadastro{
	char endereco[50];
	char nome[50];
	int cpf=0;
	char telefone[50];
	char renda[50];
	char datanasc[50];
	
	public:
	void setendereco(char frase[50]);
	void setnome(char frase[50]);
	void setcpf(int);
	int getcpf();
	string getendereco();
	string getnome();
};

class Conta{
	char senha[50];
	float money=5000;
	float limite=1000;
	Cadastro cad;
	
	public:
	//getters
	int getmoney();
	int getlimite();
	string getendereco();
	string getsenha();
	string getnome();
	int getcpf();
	//setters
	void setcpf(int val);
	void setsenha(char frase[50]);
	void setendereco(char frase[50]);
	void setnome(char frase[50]);
	void setmoney(float val);
	void setlimite(float val);
};

class Agencia{
	Conta co[50];
	char cidade[50];
	
	public:
	int getmoney(int conta);
	int getlimite(int conta);
	string getsenha(int conta);
	string getendereco(int conta);
	string getnome(int conta);
	int getcpf(int conta);
	string getcidade();
	void setcidade(char frase[50]);
	void setmoney(int,float);
	void setsenha(int,char frase[50]);
	void setendereco(int conta,char frase[50]);
	void setcpf(int conta,int);
	void setnome(int conta,char frase[50]);
	void setlimite(int conta,float val);
};

class Banco{
	Agencia ag[2];
	char senhagerente[50]="padraogerente";
	
	public:
	int getmoney(int agencia,int conta);
	int getlimite(int agencia,int conta);
	string getsenha(int agencia,int conta);
	string getendereco(int agencia,int conta);
	string getnome(int agencia,int conta);
	int getcpf(int agencia,int conta);
	string getcidade(int agencia);
	string getsenhagerente();
	void setsenhagerente(char frase[50]);
	void setsenha(int,int,char fras[50]);
	void setcidade(int agencia,char frase[50]);
	void setendereco(int,int,char frase[50]);
	void setnome(int,int,char frase[50]);
	void setcpf(int,int,int);
	void setmoney(int,int,float);
	void setlimite(int ,int ,float);
};
char globalbanco[50];
char globalsenha[50];

int main(){
	Banco b[3];
	char alteracao[50]="padrao";
	int cpfnum=0, cont, cpfdigit, subcont,subsubcont,debug,banco,bancotrans,agenciatrans,contatrans,transfval,agencia,conta;
	float intro,deposito,saque,trans,limite,x=1,gerenteagencia,gerenteconta,deflimite;
	char letra[50],senhachar[50];
	for(cont=0;cont<3;cont++){
		for(subcont=0;subcont<=1;subcont++){
			if(subcont==0){
				b[cont].setcidade(subcont,"Valinhos");
			}
			else{
				b[cont].setcidade(subcont,"Sao Paulo");
			}
		}
	}
	for(cont=0;cont<3;cont++){
		for(subcont=0;subcont<=1;subcont++){
			for(subsubcont=0;subsubcont<=10;subsubcont++){
				b[cont].setsenha(subcont,subsubcont,alteracao);
			}
		}
	}
	for(cont=0;cont<3;cont++){
		for(subcont=0;subcont<=1;subcont++){
			for(subsubcont=0;subsubcont<=4;subsubcont++){
				b[cont].setcpf(subcont,subsubcont,9999-cpfnum);
				cpfnum++;
			}
		}
	}
	
	//AGORA COMECA O PROGRAMA
	while(1){
		debug=10;
		while(debug>5){
			cout<<"Digite o nome do seu banco: (A,B ou C)"<<endl;
			cin.getline(letra,50);
			convert(letra);
			if(strcmp(globalbanco,"A")==0){ banco=0;debug=0;}
			else if(strcmp(globalbanco,"B")==0){banco=1;debug=0;}
			else if(strcmp(globalbanco,"C")==0){banco=2;debug=0;}
			else{
				cout<<"O Banco digitado nao foi encontrado!"<<endl;
				debug=10;
			}
		}
		cout<<"Voce entrou no banco: "<<globalbanco<<endl;
		cout<<"Digite:\n1->Para acessar a secao do cliente\n2->Para acessar a secao do gerente"<<endl;
		cin>>intro;
		if(intro==1){
			cout<<"Bem vindo a secao do cliente do banco "<<globalbanco<<"."<<endl;
			cout<<"Para acessar sua conta, digite o seu cpf no modelo XXXX (cpf de 4 digitos)"<<endl;
			cin>>cpfdigit;
			subsubcont=0;
			subcont=0;
			
			for(cont=0;cont<2;cont++){
				if(intro==cpfdigit){
					break;
				}
				else if(subsubcont==40){
						break;
				}
				for(subcont=0;subcont<20;subcont++){
					subsubcont++;
					intro=b[banco].getcpf(cont,subcont);
					if(intro==cpfdigit){
						cout<<"Cpf encontrado!"<<endl;
						agencia=cont;
						conta=subcont;
						break;
					}
					else{
						if(subsubcont==40){
							cout<<"Cpf nao encontrado!!"<<endl;
							break;
						}
					}
				}
			}
			cout<<"Agora digite sua senha:"<<endl;
			cin.ignore();
			cin.getline(senhachar,50);
			b[banco].getsenha(agencia,conta);
			if(strcmp(senhachar,globalsenha)==0){
				cout<<"Bem vindo a conta "<<conta+1<<" da agencia numero "<<agencia+1<<" do banco "<<globalbanco<<"."<<endl;
				cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
				cout<<"Limite: "<<b[banco].getlimite(agencia,conta)<<endl;
				cout<<"Agencia: "<<b[banco].getcidade(agencia)<<endl;
				x=1;
				while((x<6) && (x>0)){
					cout<<endl<<"Digite:\n1->Para realizar deposito\n2->Para realizar saque\n3->Para realizar transferencia\n4->Para visualizar informacoes do seu cadastro\n5->Alterar senha\n6->Sair da conta"<<endl;
					cin>>x;
					if(x==1){
						cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
						cout<<"Qual o valor que deseja depositar?"<<endl;
						cin>>deposito;
						if(deposito>0){
							intro=b[banco].getmoney(agencia,conta);
							b[banco].setmoney(agencia,conta,intro+deposito);
							cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
						}
						else{
							cout<<"Nao eh possivel realizar deposito de valores nulos ou negativos!"<<endl;
						}
					}
					else if(x==2){
						cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
						cout<<"Qual o valor que deseja sacar?"<<endl;
						cin>>saque;
						if(saque>0){
							intro=b[banco].getmoney(agencia,conta);
							limite=b[banco].getlimite(agencia,conta);
							if(saque>intro+limite){
								cout<<"Valor excedera o limite, contate o gerente"<<endl;
							}
							else{
								b[banco].setmoney(agencia,conta,intro-saque);
								cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
							}
						}
						else{
							cout<<"Nao eh possivel realizar saque de valores nulos ou negativos!"<<endl;
						}
					}
					else if(x==3){
						cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
						cout<<"Limite: "<<b[banco].getlimite(agencia,conta)<<endl;
						
						cout<<"Digite o nome do banco de destino: (A, B ou C) "<<endl;
						cin.ignore();
						cin.getline(letra,50);
						convert(letra);
						if(strcmp(globalbanco,"A")==0){ bancotrans=0;}
						else if(strcmp(globalbanco,"B")==0){bancotrans=1;}
						else if(strcmp(globalbanco,"C")==0){bancotrans=2;}
						else{
							cout<<"O Banco digitado nao foi encontrado!"<<endl;
						}
						cout<<"Agora digite o cpf da conta de destino:"<<endl;
						cin>>cpfdigit;
						subsubcont=0;
						intro=-1;
						for(cont=0;cont<2;cont++){
							if(intro==cpfdigit){
								break;
							}
							else if(subsubcont==40){
									break;
							}
							for(subcont=0;subcont<20;subcont++){
									subsubcont++;
									intro=b[banco].getcpf(cont,subcont);
									if(intro==cpfdigit){
										cout<<"Cpf encontrado!"<<endl;
										agenciatrans=cont;
										contatrans=subcont;
										break;
									}
									else{
										if(subsubcont==40){
											cout<<"Cpf nao encontrado!!"<<endl;
											break;
										}
								 }
							}
						}
						cout<<"Digite o valor a ser transferido:"<<endl;
						cin>>transfval;
						if(transfval>0){
							intro=b[banco].getmoney(agencia,conta);
							limite=b[banco].getlimite(agencia,conta);
							if(transfval<=intro+limite){
								b[banco].setmoney(agencia,conta,intro-transfval);
								intro=b[bancotrans].getmoney(agenciatrans,contatrans);
								b[bancotrans].setmoney(agenciatrans,contatrans,intro+transfval);
								cout<<"Transferencia realizada com sucesso!"<<endl;
								cout<<"Saldo: "<<b[banco].getmoney(agencia,conta);
							}
							else{
								cout<<"Valor excedera o limite, contate o gerente"<<endl;
							}
						}
						else{
							cout<<"Nao eh possivel transferir valores negativos!"<<endl;
						}
					}
					else if(x==4){
						cout<<"Seu endereco: "<<b[banco].getendereco(agencia,conta)<<endl;
						cout<<"Seu nome: "<<b[banco].getnome(agencia,conta)<<endl;
						cout<<"Seu cpf: "<<b[banco].getcpf(agencia,conta)<<endl;
						cout<<"Seu saldo: "<<b[banco].getmoney(agencia,conta)<<endl;
						cout<<"Sua agencia: "<<b[banco].getnome(agencia,conta)<<endl;
						cout<<"Sua senha: "<<b[banco].getsenha(agencia,conta)<<endl;
						cout<<"Caso tenha alguma informacao incorreta, contate seu gerente"<<endl;
					}
					else if(x==5){
						cout<<"Digite sua nova senha: "<<endl;
						cin.ignore();
						cin.getline(globalsenha,50);
						b[banco].setsenha(agencia,conta,globalsenha);
						cout<<"Senha alterada com sucesso!"<<endl;
					}
					else{
						//nada
						x=20;
					}
				}
			}
			else{
				cout<<"Senha ou cpf incorretos!"<<endl;
			}
		}
		else if(intro==2){
			cout<<"Bem vindo a secao do gerente do banco "<<globalbanco<<"."<<endl;
			cout<<"Digite sua senha:"<<endl;
			cin.ignore();
			cin.getline(senhachar,50);
			b[banco].getsenhagerente();
			if(strcmp(senhachar,globalsenha)==0){
				cout<<"Digite:\n1->Para acessar alguma conta\n2->Para criar nova conta"<<endl;
				cin>>intro;
				if(intro==1){
					cout<<"Digite o cpf da conta a ser acessada: "<<endl;
					cin>>cpfdigit;
					subsubcont=0;
					subcont=0;
					
					for(cont=0;cont<2;cont++){
						if(intro==cpfdigit){
							break;
						}
						else if(subsubcont==40){
								break;
						}
						for(subcont=0;subcont<20;subcont++){
							subsubcont++;
							intro=b[banco].getcpf(cont,subcont);
							if(intro==cpfdigit){
								cout<<"Cpf encontrado!"<<endl;
								agencia=cont;
								conta=subcont;
								break;
							}
						
							else{
								if(subsubcont==40){
									cout<<"Cpf nao encontrado!!"<<endl;
									break;
								}
							}
						}
					}
					cout<<"Bem vindo a conta "<<conta+1<<" da agencia numero "<<agencia+1<<" do banco "<<globalbanco<<"."<<endl;
					cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
					cout<<"Limite: "<<b[banco].getlimite(agencia,conta)<<endl;
					cout<<"Agencia: "<<b[banco].getcidade(agencia)<<endl;
					x=1;
					while((x<10) && (x>0)){
						cout<<endl<<"Digite:\n1->Para realizar deposito\n2->Para realizar saque\n3->Para realizar transferencia\n4->Para visualizar informacoes do cadastro da conta\n5->Alterar senha\n6->Alterar limite ou informacoesa cadastrais \n7->Sair da conta"<<endl;
						cin>>x;
						if(x==1){
							cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
							cout<<"Qual o valor que deseja depositar?"<<endl;
							cin>>deposito;
							if(deposito>0){
								intro=b[banco].getmoney(agencia,conta);
								b[banco].setmoney(agencia,conta,intro+deposito);
								cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
							}
							else{
								cout<<"Nao eh possivel realizar deposito de valores nulos ou negativos!"<<endl;
							}
						}
						else if(x==2){
							cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
							cout<<"Qual o valor que deseja sacar?"<<endl;
							cin>>saque;
							if(saque>0){
								intro=b[banco].getmoney(agencia,conta);
								limite=b[banco].getlimite(agencia,conta);
								if(saque>intro+limite){
									cout<<"Valor excedera o limite"<<endl;
								}
								else{
									b[banco].setmoney(agencia,conta,intro-saque);
									cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
								}
							}
							else{
								cout<<"Nao eh possivel realizar saque de valores nulos ou negativos!"<<endl;
							}
						}
						else if(x==3){
							cout<<"Saldo atual: "<<b[banco].getmoney(agencia,conta)<<endl;
							cout<<"Limite: "<<b[banco].getlimite(agencia,conta)<<endl;
							
							cout<<"Digite o nome do banco de destino: (A, B ou C) "<<endl;
							cin.ignore();
							cin.getline(letra,50);
							convert(letra);
							if(strcmp(globalbanco,"A")==0){ bancotrans=0;}
							else if(strcmp(globalbanco,"B")==0){bancotrans=1;}
							else if(strcmp(globalbanco,"C")==0){bancotrans=2;}
							else{
								cout<<"O Banco digitado nao foi encontrado!"<<endl;
							}
							cout<<"Agora digite o cpf da conta de destino:"<<endl;
							cin>>cpfdigit;
							subsubcont=0;
							for(cont=0;cont<2;cont++){
								if(intro==cpfdigit){
									break;
								}
								else if(subsubcont==40){
										break;
								}
								for(subcont=0;subcont<20;subcont++){
										subsubcont++;
										intro=b[banco].getcpf(cont,subcont);
										if(intro==cpfdigit){
											cout<<"Cpf encontrado!"<<endl;
											agenciatrans=cont;
											contatrans=subcont;
											break;
										}
										else{
											if(subsubcont==40){
												cout<<"Cpf nao encontrado!!"<<endl;
												break;
											}
									 }
								}
							}
							cout<<"Digite o valor a ser transferido:"<<endl;
							cin>>transfval;
							if(transfval>0){
								intro=b[banco].getmoney(agencia,conta);
								if(transfval>intro+limite){
									b[banco].setmoney(agencia,conta,intro-transfval);
									intro=b[bancotrans].getmoney(agenciatrans,contatrans);
									b[bancotrans].setmoney(agenciatrans,contatrans,intro+transfval);
									cout<<"Transferencia realizada com sucesso!"<<endl;
									cout<<b[banco].getmoney(agencia,conta);
								}
								else{
									cout<<"Valor excedera o limite"<<endl;
								}
							}
							else{
								cout<<"Nao eh possivel transferir valores negativos!"<<endl;
							}
						}
						else if(x==4){
							cout<<"Endereco da conta: "<<b[banco].getendereco(agencia,conta)<<endl;
							cout<<"Nome do cliente: "<<b[banco].getnome(agencia,conta)<<endl;
							cout<<"Cpf do cliente: "<<b[banco].getcpf(agencia,conta)<<endl;
							cout<<"Saldo do cliente: "<<b[banco].getmoney(agencia,conta)<<endl;
							cout<<"Agencia do cliente: "<<b[banco].getnome(agencia,conta)<<endl;
							cout<<"Senha da conta: "<<b[banco].getsenha(agencia,conta)<<endl;
						}
						else if(x==5){
							cout<<"Digite a nova senha: "<<endl;
							cin.ignore();
							cin.getline(globalsenha,50);
							b[banco].setsenha(agencia,conta,globalsenha);
							cout<<"Senha alterada com sucesso!"<<endl;
						}
						else if(x==6){
							cout<<"Digite: \n1->Para alterar o limite\n2->Para alterar o endereco\n3->Para alterar o nome do cliente\n4->Para alterar o cpf do conta\n5->Para cancelar operacao"<<endl;
							cin>>x;
							if(x==1){
								cout<<"Digite o novo limite"<<endl;
								cin>>deflimite;
								if(deflimite<0){
									cout<<"O limite nao pode ser um numero negativo!"<<endl;
								}
								else{
									b[banco].setlimite(agencia,conta,deflimite);
								}
							}
							else if(x==2){
								cout<<"Digite o novo endereco: "<<endl;
								cin.ignore();
								cin.getline(globalsenha,50);
								b[banco].setendereco(agencia,conta,globalsenha);
							}
							else if(x==3){
								cout<<"Digite o novo nome do cliente da conta: "<<endl;
								cin.ignore();
								cin.getline(globalsenha,50);
								b[banco].setnome(agencia,conta,globalsenha);
							}
							else if(x==4){
								cout<<"Digite o novo cpf da conta: "<<endl;
								cin>>cpfdigit;
								b[banco].setcpf(agencia,conta,cpfdigit);
							}
						}
						else{
							//nada
							x=20;
						}
					}
					
				}
				else if(intro==2){
					cout<<"Digite:\n1-> Para criar uma nova conta na agencia de VALINHOS\n2->Para criar uma nova conta na agencia de SAO PAULO"<<endl;
					cin>>gerenteagencia;
					gerenteagencia=gerenteagencia-1;
					for(cont=0;true;cont++){
						intro=b[banco].getcpf(gerenteagencia,cont);
						if(intro==0){
							gerenteconta=cont;
							break;
						}
					}
					cout<<"Numero da conta: "<<gerenteconta+1<<endl;
					cout<<"Digite o cpf da nova conta: "<<endl;
					cin>>cpfdigit;
					b[banco].setcpf(gerenteagencia,gerenteconta,cpfdigit);
					cout<<"Digite a senha para a conta: "<<endl;
					cin.ignore();
					cin.getline(senhachar,50);
					b[banco].setsenha(gerenteagencia,gerenteconta,senhachar);
					cout<<"Conta criada com sucesso!"<<endl;
				}
			}
			else{
				cout<<"Senha incorreta!"<<endl;
			}
		}
    }
	system("pause");
	return 0;
}
/*********************************************/
void Banco::setsenhagerente(char frase[50]){
	strcpy(senhagerente,frase);
}

string Banco::getsenhagerente(){
	strcpy(globalsenha,senhagerente);
	return senhagerente;
}
/*********************************************/
void Banco::setmoney(int agencia, int conta, float val){
	ag[agencia].setmoney(conta,val);
}

void Agencia::setmoney(int conta,float val){
	co[conta].setmoney(val);
}

void Conta::setmoney(float val){
	money=val;
}

int Banco::getmoney(int agencia,int conta){
	 return ag[agencia].getmoney(conta);
}

int Agencia::getmoney(int conta){
	return co[conta].getmoney();
}

int Conta::getmoney(){
	return money;
}
/*******************************************************/
void Banco::setsenha(int agencia, int conta, char frase[50]){
	ag[agencia].setsenha(conta,frase);
}

void Agencia::setsenha(int conta,char frase[50]){
	co[conta].setsenha(frase);
}

void Conta::setsenha(char frase[50]){
	strcpy(senha,frase);
}

string Banco::getsenha(int agencia,int conta){
	 return ag[agencia].getsenha(conta);
}

string Agencia::getsenha(int conta){
	return co[conta].getsenha();
}

string Conta::getsenha(){
	strcpy(globalsenha,senha);
	return senha;
}
/*******************************************************/
void Banco::setlimite(int agencia,int conta,float val){
	ag[agencia].setlimite(conta,val);
}

void Agencia::setlimite(int conta,float val){
	co[conta].setlimite(val);
}

void Conta::setlimite(float val){
	limite=val;
}

int Banco::getlimite(int agencia,int conta){
	 return ag[agencia].getlimite(conta);
}

int Agencia::getlimite(int conta){
	return co[conta].getlimite();
}

int Conta::getlimite(){
	return limite;
}
/*************************************************/
void Banco::setendereco(int agencia,int conta,char frase[50]){
	ag[agencia].setendereco(conta,frase);
}

void Agencia::setendereco(int conta,char frase[50]){
	co[conta].setendereco(frase);
}

void Conta::setendereco(char frase[50]){
	cad.setendereco(frase);
}

void Cadastro::setendereco(char frase[50]){
	strcpy(endereco,frase);
}

string Banco::getendereco(int agencia,int conta){
	 return ag[agencia].getendereco(conta);
}

string Agencia::getendereco(int conta){
	return co[conta].getendereco();
}

string Conta::getendereco(){
	return  cad.getendereco();
}

string Cadastro::getendereco(){
	return endereco;
}
/*************************************************/
void Banco::setnome(int agencia,int conta,char frase[50]){
	ag[agencia].setnome(conta,frase);
}

void Agencia::setnome(int conta,char frase[50]){
	co[conta].setnome(frase);
}

void Conta::setnome(char frase[50]){
	cad.setnome(frase);
}

void Cadastro::setnome(char frase[50]){
	strcpy(nome,frase);
}

string Banco::getnome(int agencia,int conta){
	 return ag[agencia].getnome(conta);
}

string Agencia::getnome(int conta){
	return co[conta].getnome();
}

string Conta::getnome(){
	return  cad.getnome();
}

string Cadastro::getnome(){
	return nome;
}
/**************************************************/
void Banco::setcpf(int agencia,int conta,int val){
	ag[agencia].setcpf(conta,val);
}

void Agencia::setcpf(int conta,int val){
	co[conta].setcpf(val);
}

void Conta::setcpf(int val){
	cad.setcpf(val);
}

void Cadastro::setcpf(int val){
	cpf=val;
}

int Banco::getcpf(int agencia,int conta){
	 return ag[agencia].getcpf(conta);
}

int Agencia::getcpf(int conta){
	return co[conta].getcpf();
}

int Conta::getcpf(){
	return  cad.getcpf();
}

int Cadastro::getcpf(){
	return cpf;
}
/**********************************************/
void Banco::setcidade(int agencia,char frase[50]){
	ag[agencia].setcidade(frase);
}
void Agencia::setcidade(char frase[50]){
	strcpy(cidade,frase);
}
string Banco::getcidade(int agencia){
	return ag[agencia].getcidade();
}
string Agencia::getcidade(){
	return cidade;
}
/**********************************************/
void convert(char converter[50]){
	int cont=0;
	while(converter[cont]!='\0'){
		if(converter[cont]>='a' && converter[cont]<='z'){
			converter[cont]=converter[cont]-'a'+'A';
		}
		cont++;
	}
	strcpy(globalbanco,converter);
}
