#############################################################################################
       Trabalho Prático de Redes de Computadores I - Implementação da camada física        
                                                                                           
 2017/2 - 6º período                                                                       
                                                                                           
 Gabriel Pires Miranda de Magalhães    -   201422040011                                    
 Vinícius Magalhães D'Assunção         -   201422040232                                    
 Thayane Pessoa Duarte                 -   201312040408                                    
 Victor de Oliveira Balbo              -   201422040178                                    
#############################################################################################

# Decisões de implementação
	Foi utilizado arquivos para comunicar entre as camadas do Cliente e para comunicar entre as camadas do Servidor. Foi feita esta escolha para não precisar executar um terminal para cada camada e ficar ouvindo e cada camada ficar ouvindo uma porta. No projeto, ao precisar de uma camada, a camada atual executa o script da camada desejada e escreve sua PDU em um arquivo.

	Na camada de aplicação, é feita a comunicação via socket com o browser, em que a porta é definida pelo usuário. O cabeçalho da camada de aplicação contém os seguintes campos: método, ip, hostname, porta e arquivo solicitado. Ela escreve seu cabeçalho em um arquivo chamado segmento.

	A camada de transportes adiciona os seguintes cabeçalhos no segmento quando protocolo utilizado é o UDP: número de sequência, porta origem, porta destino, tamanho do segmento e checksum. Escreve o segmento em um arquivo. No TCP, adiciona os seguintes campos ao cabeçalho: número de sequência, ack, porta origem, porta destino, tamanho da janela de transferência e tamanho do cabeçalho. Quando o protocolo utilizado é o UDP, a camada de transporte já divide a mensagem e envia os segmentos para o servidor de uma vez. Quando o protocolo é o TCP, a camada envia um segmento para estabelecer a conexão, espera a resposta, envia os segmentos referente a mensagem recebida da aplicação com a requisição, espera a resposta e finalmente envia um segmento para encerrar a conexão para a camada física e a camada de transporte do servidor encerra a conexão. No servidor quando é pedido a página, ele encaminha o pedido para a camada de aplicação do servidor. A camada de trasportes ao repartir a mensagem em segmentos adiciona um identificador de fim da mensagem no segmento correspondente. Cada segmento tem um número em seu nome que é o número de sequência do mesmo.

	A camada de rede divide os segmentos em pacotes, adiciona um identificador de fim de segmento no pacote correspondente. Escreve os pacotes em um arquivo chamado pacote. Cada pacote tem um número referente ao número de sequência do pacote. Cada pacote possui um número em seu nome para indicar a seguência.

	A camada física sempre que se comunica com a outra camada física via socket utilizando uma porta para o cliente e outra para o servidor. Ela necessita de dar um sleep antes da comunicação e necessita da confirmação de recebimento do que foi solicitado para fins de sincronização e para os quadros não se perderem. A comunicação acontece da seguinte maneira:
		1 - Uma camada fica escutando esperando a solicitação, quando a outra camada faz a solicitação passa a escutar sua porta, esperando a resposta. 
		Então a camada que recebeu a solicitação envia o que foi solicitado e escuta a sua porta.
		2 - A camada que fez a solicitação ao receber o que foi solicitado envia o que foi recebido para a outra camada e volta a escutar sua porta.
		3 - A camada que enviou o que foi solicitado compara o que foi enviado com o que foi recebido e se forem iguais, envia um OK para a outra camada e segue para o próximo passo. Se não forem iguais gera um erro e volta para o início da execução da camada.
		4 - A camada que fez a solicitação recebe a confirmação. Se estiver OK, segue para a próxima etapa da execução, senão gera uma mensagem de erro e volta para o início da execução.
	A camada física primeiro envia o IP e porta do cliente, envia o tamanho máximo dos quadros, parte os pacotes em quadros e os envia.


# Requisitos
	* netcat
		sudo apt-get install -y netcat
	* php5
		sudo apt-get install -y php5-common libapache2-mod-php5 php5-cli
	* nodejs
		sudo apt-get install -y node
	* sheljs
		- Entre nas pastas "Servidor" e "Cliente"
		- Digite o comando no terminal:
		sudo npm install -g shelljs


# Instruções de execução:
	1 - Abra dois terminais do linux. Ou um terminal em cada computador. Coloque a pasta "Cliente" e a pasta "Servidor" nos devidos computadores. 

	2 - Atribua permissão a todos os scripts
		2.1 - Entre na pasta "Cliente" e execute o comando:
			chmod +x *
		2.2 - Entre na pasta "Servidor" e execute o comando:
			chmod +x *

	2 - No primeiro terminal, acesse a pasta "Servidor" e execute no terminal:
		./fis_server.sh <porta_camada_fisica_servidor>
			<porta_camada_fisica_servidor>

	3 - No segundo terminal, acesse a pasta "Cliente" e execute o seguinte comando no terminal:
		./fis_server.sh <protocolo> <ip_servidor> <porta_camada_fisica_servidor> <porta_web>
		- <protocolo>: indica o protocolo utilizado na camada de transportes. Pode ser udp ou tcp. Deve estar em letras minúsculas.

	4 - Abra o BROWSER no computador onde está o cliente:
		4.1 - O endereço se consiste em "IP local":"PORTA"/"PAGINA", os utilizados foram: 
			4.1.1 - 127.0.0.1:8080/cliente
			4.1.2 - 127.0.0.1:8080/simples
			4.1.4 - 127.0.0.1:8080/<QualquerNome> que mostra pagina não encontrada.
		4.2 - Acesse o endereço que desejar.
		4.3 - A página será então exibida.


# Fluxo:
	1 - O BROWSER solicita para camada de aplicação de Cliente a página. Vá para o passo 2.
	
	2 - A camada de aplicação do Cliente que estava escutando recebe a requisição, monta o cabeçalho da mensagem e solicita a página para a camada de transporte, enviando a mensagem para a mesma. Vá para o passo 3. 
	
	3 - A camada de transportes do Cliente recebe a mensagem da camada de aplicação, que tem funcionamento diferente dependendo do protocolo utilizado:
		3.1 - UDP:
			3.1.1 - Divide a mensagem em segmentos, colocando seu cabeçalho. Coloca um identificador de fim da mensagem no último segmento. Envia os segmentos para a camada de rede. Vá para o passo 4. 

		3.2 - TCP:
			3.2.1 - Cria um segmento com a string "conexao" na parte de dados para estabelecer conexão. Coloca o cabeçalho e envia o segmento para a camada de rede. Vá para o passo 4.

	4 - A camada de rede divide os segmentos em pacotes e coloca um identificador de fim do pacote no último pacote referente ao segmento. Envia os pacotes para a camada física. Vá para o passo 5.
	
	5 - A camada física do Cliente estabele conexão com a camada física do servidor, divide os pacotes em quadros, colocando seu cabeçalho e envia para a camada física do Servidor. Segue para o passo 6 e volta a escutar a porta especificada.

	6 - A camada física do servidor recebe os quadros do servidor, retira o seu cabeçalho, reconstroi os pacotes e envia-os para a camada de rede.
	
	7 - A camada de rede do cliente recebe os pacotes, retira seu cabeçalho, reconstroi os segmentos e envia-os para a camada de transporte.

	8 - A camada de transporte possui diferente funcionamento dependendo do protocolo utilizado:
		8.1 - UDP:
			8.1.1 - Recebe os segmentos da camada de Rede, reconstroi a mensagem e a envia para a camada de aplicação. Segue para o passo 9.
		8.2 - TCP:
			8.2.1 - Verifica o conteúdo do segmento recebido. Se for uma solicitação de estabelecimento de conexão vai, estabele a conexão, monta um segmento, coloca seu cabeçalho e coloca um "OK" na parte de dados. Se não for uma solicitação de estabelecimento de conexão vai para o passo 8.2.2.
			8.2.2 - Verifica o conteúdo do segmento recebido. Se for uma requisição da camada de aplicação, reconstroi a mensagem com os segmentos recebidos e a envia para a camada de aplicação. Segue para o passo 9. Se tiver recebido um ack no segmento segue para o passo 8.2.3.
			8.2.3 - Encerra sua execução. E todos as camadas exceto a física do servidor vão encerrando sua execução até chegar na camada de transportes do cliente.  

	9 - A camada de aplicação recebe a mensagem, lê a requisição da mensagem, monta uma mensagem com a página solicitada e envia para a camada de transportes e encerra sua execução. Segue para o passo 10.
	
	10 - A camada de transporte do servidor recebe mensagem da camada de aplicação, monta os segmentos, colocando seu cabeçalho e envia para a camada de rede. Segue para o passo 11.
	
	11 - A camada de rede do servidor faz a mesma coisa que o passo 4 só que no servidor. Encerra sua execução. Segue para o passo 12.

	12 - A camada física do servidor faz a mesma coisa que o passo 5 só que estabele conexão e envia quadros para o cliente. Não encerra a execução. Segue para o passo 13.

	13 - A camada física do cliente faz a mesma coisa que o passo 6 só que no cliente. Encerra sua execução. Segue para o passo 14.

	14 - A camada de rede do cliente faz a mesma coisa que o passo 7 só que no cliente. Encerra sua execução. Segue para o passo 15.

	15 - A camada de transporte do Cliente possui um funcionamento diferente para cada protocolo:
		15.1 - UDP:
			15.1.1 - Retira o seu cabeçalho, reconstroi a mensagem, envia para a camada aplicação, encerra sua execução e vai para o passo 16.

		15.2 - TCP:
			15.2.1 - Verifica se foi estabelecida a conexão com a camada de transporte do servidor. Se sim, pega a mensagem que tinha recebido da camada de aplicação, divide em segmentos colocando seu cabeçalho e envia para a camada de rede. Vá para o passo 4. Senão encerra a execução. Se o segmento não tiver o estabelecimento de conexão e sim a página solicitada, segue para o passo 15.2.2.
			15.2.2 - Cria um segmento com ack e envia para a camada de redes e vai para o passo 15.2.3.
			15.2.3 - Monta a mensagem com os segmentos recebidos em 15.2.2 e envia para a camada de aplicação. Segue para o passo 16.

	16 - A camada de aplicação do Cliente entrega o http para o BROWSER e volta a escutar a porta especificada.