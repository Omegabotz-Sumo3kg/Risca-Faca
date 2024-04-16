# Risca-Faca

## Configurações e Instalação

No arduino IDE é necessário ter instalado para compilar o código:

- Na aba "Preferences", em "Additional boards manager URLs" adicionar esta URL: "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"

- Instalar lib IRRemote na versão *4.2.0* (exatamente essa)

- Instalar lib PS4Controller na versão *2.1* (mais recente)

- Instlar lib ESP32Servo na versão *1.1.2* (mais recente)

<br/>
Para instalar corretamente a lib do ps4 você precisará:

- Baixar o arquivo zipado através do link: "[https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbUw2TW9SYzJvbU5GZlRkWkp..](https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbUw2TW9SYzJvbU5GZlRkWkpVRm9NQUstRnVLQXxBQ3Jtc0tuTURWOUhtTGd5NV9Rck5jNi1oWmExTWJ1NmxVOEZNTTVKWm9WU2RxMWpYTXo4YWNvaVdfSUxzbXg1ZlZNdU5Fa3prMHcxdC1sbWxCRnFqUFc3RlRPeEc3a2M1ZXZFeUh1cUcySTVxOXU4czRsLWV6OA&q=https%3A%2F%2Fgithub.com%2Faed3%2FPS4-esp32%2Farchive%2Frefs%2Fheads%2Fmaster.zip&v=dRysvxQfVDw)."

- Adicionar a lib na aba "Sketch", depois em "Include library", em seguida "ADD .ZIP library" e selecionar o arquivo zipado baixado anteriormente,

- Após adicionar a lib será necessário ir até o arquivo ps4_spp dentro do arquivo zipado e comentar as quatro linhas (como está na imagem abaixo).

![image](https://github.com/rezina182/Risca-Faca/assets/92062493/692cb40d-f4ae-4430-b359-69ef3ffb153f)

- Nesse vídeo, mostra as linhas de um dos códigos da biblioteca PS4Controller que precisa comentar e na descrição o link para baixar o arquivo zipado: https://www.youtube.com/watch?v=dRysvxQfVDw

<br/>
Dentro da Arduino IDE é necessário configurar da seguinte forma:

- Em "Select Other Board and Port", selecionar: *"DOIT ESP32 DEVKIT V1"*
![image](https://github.com/rezina182/Risca-Faca/assets/92062493/d26dbfec-993e-4458-953e-76a768252743)

