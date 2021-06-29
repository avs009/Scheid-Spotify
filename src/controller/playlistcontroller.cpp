#include "playlistcontroller.h"

#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../exceptions/exception.h"
#include "trackcontroller.h"

PlayList PlayListController::convertPlayListFromJson(const QJsonValue & playlistInJSON)
{
    if(!playlistInJSON.isObject())
        throw Exception("JSON de uma playlist inválido!");

    // Converte o valor JSON para um objeto em JSON
    auto object = playlistInJSON.toObject();

    PlayList playlist;

    // Faz o parser de cada um dos atributos
    playlist.setName(object.value("name").toString());

    // Faz o parser dos items inclusos no objeto
    auto items = object.value("items");

    // Verifica se o valor encontrado é do tipo array como esperado
    if(items.isArray())
    {
        // Converte o valor em JSON para uma array de valores JSON
        auto itemsArray = items.toArray();

        // Para cada valor JSON tentar realizar o parser pela função de acordo com o tipo do item
        foreach(const QJsonValue & value, itemsArray)
        {
            // Realiza o parser de cada um dos itens como objeto json
            playlist.addTrack(TrackController::convertTrackFromJSON(value));
        }
    }

    return playlist;
}

QJsonValue PlayListController::convertPlayListToJson(const PlayList & playlist)
{
    QJsonObject playlistInJson;

    playlistInJson["name"] = playlist.getName();

    QJsonArray items;
    foreach(auto t, playlist.getTracks())
    {
        items.append(TrackController::convertTrackToJSON(t));
    }
    playlistInJson["items"] = items;

    return playlistInJson;
}

PlayList PlayListController::loadPlayList(QFile & file)
{
    if(!file.exists() || file.size() == 0)
        throw Exception("Arquivo de playlist inválido: " + file.fileName());

    // Se não for possível abrir o arquivo, retorne uma exceção com o erro
    if(!file.open(QFile::ReadOnly))
        throw Exception(file.errorString());

    auto jsonFromFile = file.readAll();
    file.close();

    if(jsonFromFile.size() == 0)
        throw Exception("Arquivo de playlist vazio ou erro na leitura!");

    const auto document = QJsonDocument::fromJson(jsonFromFile);
    if(!document.isObject())
        throw Exception("JSON Inválido no arquivo de playlist!");

    // Converte o JSON lido do arquivo para o objeto playlist
    auto playlist = convertPlayListFromJson(document.object());
    playlist.setFile(file.fileName());

    return playlist;
}

void PlayListController::savePlayList(const PlayList & playlist)
{
    auto fileName = playlist.getFile();
    if(fileName.isEmpty())
        throw Exception("Arquivo inválido para salvar a playlist!");

    QFile file(fileName);

    if(!file.open(QFile::WriteOnly))
        throw Exception("Falha ao abrir o arquivo para escrita!");

    QJsonDocument document(convertPlayListToJson(playlist).toObject());

    // Se ao escrever o documento json identado no arquivo der erro
    if(file.write(document.toJson(QJsonDocument::Indented)) == -1)
    {
        file.close();
        throw Exception("Falha ao escrever no arquivo da playlist!");
    }

    file.close();
}
