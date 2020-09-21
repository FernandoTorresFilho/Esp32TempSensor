# Esp32TempSensor
Projeto da disciplina Sistemas MicroControlados. 
1 Termômetro digital
Deverá ser desenvolvido um termômetro digital com as seguintes características. A cada 30 s uma nova medição deve ser realizada, ou quando um botão especial (LER AGORA) for pressionado.
Cada coleta de temperatura deverá ser apresentada em um display LCD e após 10 s o display deve apagar. A resolução da apresentação das medições deve ser de 0,1°C.
Na última linha do display LCD deverá conter o histórico das últimas 3 medições em ordem cronológica, separadas por 2 espaços vazios. A cada nova coleta, o dado mais antigo é excluído do display.
Deverá haver um botão de reset geral que limpa qualquer dado de temperatura guardado na memória do microcontrolador. Neste caso, novas medições de 30 segundos deverão ser reiniciadas a partir deste momento.
