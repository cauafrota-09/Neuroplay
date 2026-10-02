const formCadastro = document.getElementById("formCadastro");

const nome = document.getElementById("nome");
const email = document.getElementById("email");
const senha = document.getElementById("senha");
const confirmarSenha = document.getElementById("confirmarSenha");

const mensagem = document.getElementById("mensagem");


/* ==============================
   MOSTRAR / OCULTAR SENHA
================================= */

const botoesMostrarSenha = document.querySelectorAll(".mostrar-senha");

botoesMostrarSenha.forEach((botao) => {

    botao.addEventListener("click", () => {

        const idInput = botao.dataset.input;

        const input = document.getElementById(idInput);


        if (input.type === "password") {

            input.type = "text";
            botao.textContent = "Ocultar";

        } else {

            input.type = "password";
            botao.textContent = "Mostrar";

        }

    });

});


/* ==============================
   CADASTRO
================================= */

formCadastro.addEventListener("submit", (event) => {

    event.preventDefault();


    /* Limpa mensagens anteriores */

    mensagem.textContent = "";

    mensagem.classList.remove(
        "mensagem-erro",
        "mensagem-sucesso"
    );


    /* ==============================
       VALIDAÇÃO DO NOME
    ================================= */

    if (nome.value.trim() === "") {

        mostrarErro("Digite seu nome.");

        return;

    }


    /* ==============================
       VALIDAÇÃO DA SENHA
    ================================= */

    if (senha.value.length < 6) {

        mostrarErro(
            "A senha deve possuir pelo menos 6 caracteres."
        );

        return;

    }


    /* ==============================
       CONFIRMAÇÃO DA SENHA
    ================================= */

    if (senha.value !== confirmarSenha.value) {

        mostrarErro(
            "As senhas não coincidem."
        );

        return;

    }


    /* ==============================
       CADASTRO VÁLIDO
    ================================= */

    mensagem.textContent =
        "Cadastro validado com sucesso!";

    mensagem.classList.add(
        "mensagem-sucesso"
    );


    console.log("Dados do cadastro:");

    console.log({
        nome: nome.value.trim(),
        email: email.value.trim()
    });


    /*
        FUTURAMENTE:

        Aqui será feita a integração
        com o Firebase Authentication.
    */

});


/* ==============================
   FUNÇÃO DE ERRO
================================= */

function mostrarErro(texto) {

    mensagem.textContent = texto;

    mensagem.classList.add(
        "mensagem-erro"
    );

}