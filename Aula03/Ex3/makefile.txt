#A C D
#B E F G

#Lucas Bagatini do Nascimento


# make X vai rodar a resposta do item X do exercício 3.


#limpa o diretório dos resultados anteriores
clear:
	rm *.o *.a *.so


a:
	#foi feito à mão

b:
	#cria libteststa.a a partir do novo test.c
	cc test.c -c -Wall
	ar cr libteststa.a test.o

	#cria libtestdyn.so a partir do novo test.c
	cc test.c -c -Wall -fPIC
	cc -o libtestdyn.so test.o  -shared -fPIC

c1:
	# appdyn retorna 4 pois ele referencia uma biblioteca dinamicamente. Desse modo, se a biblioteca que ele referencia mudar, seu comportamento muda.
	./appdyn

c2:
	# appsta retorna 3 pois ele utiliza uma biblioteca que foi estaticamente incluída dentro de si. Desse modo,seu comportamento é independente de modificações à biblioteca original, já que possui uma cópia da versão passada.
	./appsta






#prepara o diretório atual para ficar igual ao do ex2
ready:
	make 2a
	make 2b
	make 2c
	make 2d
	make 2e
	make 2f
	make 2g

2a:
	#cria o código-objeto , ou objeto relocável
	cc test.c -c -Wall
	#cria um Archive (ficheiro?) com os .o
	ar cr libteststa.a test.o

2b:
	#cria o código-objeto , ou objeto relocável, pronto pra ser agregado ao Shared Object
	cc test.c -c -Wall -fPIC
	#cria um Shared Object com os .o
	cc -o libtestdyn.so test.o  -shared -fPIC

2c:
	#cria o código-objeto , ou objeto relocável
	cc -o appsta.o app.c -c
	# faz a linkagem de appsta.o e o archive libteststa.a
	cc -o appsta appsta.o -L. -lteststa

2d:
	#já joga na tela o retorno
	./appsta

	#make não gosta mto disso, aparentemente
	echo $?

2e:
	#cria o código-objeto , ou objeto relocável, ao qual será linkado o .so
	cc -o appdyn.o app.c -c -Wall
	# faz a linkagem de appdyn.o e o shared object libtestdyn.so
	cc -o appdyn appdyn.o -L. -ltestdyn -Wl,-rpath,$(CURDIR)
	# $(CURDIR) é o diretório de onde o make está sendo executado. Dá certo so se essa diretriz for exectada do msm diretório

2f:
	#já joga na tela o retorno
	./appdyn

	#make não gosta mto disso, aparentemente
	echo $?

2g:
	#dependencias de appdyn
	ldd appdyn
	#dependencias de appsta
	ldd appsta
	# fica claro que Appdyn depende da biblioteca compartilhada criada durante o exercício (libtestdyn.so) enquanto appsta não, já que suas dependências foram inseridas dentro de si (semelhante à compilação estática usando -static)
