
#Lucas Bagatini do Nascimento


# make X vai rodar a resposta do item X do exercício 1.


#limpa o diretório dos resultados anteriores
clear:
	rm *.o *.a app

a:
	#cria o código-objeto , ou objeto relocável
	#obj relocável é código de máquina incompleto, não linkado
	cc test.c -c -Wall
	#cria um Archive (ficheiro?) com os .o
	#o Archive é chamado libtest.a e a "biblioteca" associada é ltest
	ar cr libtest.a test.o
	#faz a linkagem entre o código-objeto app.o (q tem a main) e as bibliotecas (nesse acso libtest) e termina a compilação

b:
	cc app.c -c -Wall

c1:
	# faz a linkagem na ordem correta!
	cc -o app app.o -L./ -ltest

c2:
	# faz a linkagem na ordem errada!

	# de acordo com o manual do gcc:
	# " It makes a difference where in the command you write this option [-L]; the linker searches and processes libraries and object files in the order they are specified. Thus, `foo.o -lz bar.o' searches library `z' after file foo.o but before bar.o. If bar.o refers to functions in `z', those functions may not be loaded. "
	# https://gcc.gnu.org/onlinedocs/gcc-4.4.2/gcc/Link-Options.html#Link-Options

	cc -o app -L./ -ltest app.o


working:
	#cria o código-objeto , ou objeto relocável
	#obj relocável é código de máquina incompleto, não linkado
	cc *.c -c -Wall
	#cria um Archive (ficheiro?) com os .o
	#o Archive é chamado libtest.a e a "biblioteca" associada é ltest
	ar cr libtest.a test.o
	#faz a linkagem entre o código-objeto app.o (q tem a main) e as bibliotecas (nesse acso libtest) e termina a compilação
	# faz a linkagem na ordem correta!
	cc -o app app.o -L./ -ltest

broken:
	#cria o código-objeto , ou objeto relocável
	#obj relocável é código de máquina incompleto, não linkado
	cc *.c -c -Wall
	#cria um Archive (ficheiro?) com os .o
	#o Archive é chamado libtest.a e a "biblioteca" associada é ltest
	ar cr libtest.a test.o
	#faz a linkagem entre o código-objeto app.o (q tem a main) e as bibliotecas (nesse acso libtest) e termina a compilação
	# faz a linkagem na ordem errada!

	# de acordo com o manual do gcc:
	# " It makes a difference where in the command you write this option [-L]; the linker searches and processes libraries and object files in the order they are specified. Thus, `foo.o -lz bar.o' searches library `z' after file foo.o but before bar.o. If bar.o refers to functions in `z', those functions may not be loaded. "
	# https://gcc.gnu.org/onlinedocs/gcc-4.4.2/gcc/Link-Options.html#Link-Options

	cc -o app -L./ -ltest app.o
