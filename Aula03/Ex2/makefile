#A C D
#B E F G

#Lucas Bagatini do Nascimento


# make X vai rodar a resposta do item X do exercício 2.


#limpa o diretório dos resultados anteriores
clear:
	rm *.o *.a appdyn* appsta* *.so

a:
	#cria o código-objeto , ou objeto relocável
	cc test.c -c -Wall
	#cria um Archive (ficheiro?) com os .o
	ar cr libteststa.a test.o

b:
	#cria o código-objeto , ou objeto relocável, pronto pra ser agregado ao Shared Object
	cc test.c -c -Wall -fPIC
	#cria um Shared Object com os .o
	cc -o libtestdyn.so test.o  -shared -fPIC

c:
	#cria o código-objeto , ou objeto relocável
	cc -o appsta.o app.c -c
	# faz a linkagem de appsta.o e o archive libteststa.a
	cc -o appsta appsta.o -L. -lteststa

d:
	#já joga na tela o retorno
	./appsta

	#make não gosta mto disso, aparentemente
	echo $?

e:
	#cria o código-objeto , ou objeto relocável, ao qual será linkado o .so
	cc -o appdyn.o app.c -c -Wall
	# faz a linkagem de appdyn.o e o shared object libtestdyn.so
	cc -o appdyn appdyn.o -L. -ltestdyn -Wl,-rpath,$(CURDIR)
	# $(CURDIR) é o diretório de onde o make está sendo executado. Dá certo so se essa diretriz for exectada do msm diretório

f:
	#já joga na tela o retorno
	./appdyn

	#make não gosta mto disso, aparentemente
	echo $?

g:
	#dependencias de appdyn
	ldd appdyn
	#dependencias de appsta
	ldd appsta
	# fica claro que Appdyn depende da biblioteca compartilhada criada durante o exercício (libtestdyn.so) enquanto appsta não, já que suas dependências foram inseridas dentro de si (semelhante à compilação estática usando -static)
