#include "searchcontrollertest.h"

#include "src/controller/pagingcontroller.h"
#include "src/controller/searchcontroller.h"

#include "src/exceptions/exception.h"

SearchControllerTest::SearchControllerTest(QObject *parent) : QObject(parent), spotifyWrapper(nullptr)
{

}

void SearchControllerTest::testSearchAny()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Não Para",SearchController::SearchFilterType::Any});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    QString error = arguments.at(2).value<QString>();

    QString printError = "Falha na requisição: " + error;

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchName()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Minina má",SearchController::SearchFilterType::Name});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    QString error = arguments.at(2).value<QString>();

    QString printError = "Falha na requisição: " + error;

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchArtist()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Anitta",SearchController::SearchFilterType::Artist});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    int status = arguments.at(1).toInt();
    QString error = arguments.at(2).value<QString>();
    QString errorMessage = arguments.at(3).value<QString>();

    QString printError = "Falha na requisição:";
    QTextStream(&printError) << " httpStatus=" << status
                             << " reqError='" << error
                             << "' errMsg='" << errorMessage
                             << "'";

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchAlbum()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Meu Lugar",SearchController::SearchFilterType::Album});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    int status = arguments.at(1).toInt();
    QString error = arguments.at(2).value<QString>();
    QString errorMessage = arguments.at(3).value<QString>();

    QString printError = "Falha na requisição:";
    QTextStream(&printError) << " httpStatus=" << status
                             << " reqError='" << error
                             << "' errMsg='" << errorMessage
                             << "'";

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchNameAndArtist()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Não Para",SearchController::SearchFilterType::Name});
    filters.append({"Anitta",SearchController::SearchFilterType::Artist});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    int status = arguments.at(1).toInt();
    QString error = arguments.at(2).value<QString>();
    QString errorMessage = arguments.at(3).value<QString>();

    QString printError = "Falha na requisição:";
    QTextStream(&printError) << " httpStatus=" << status
                             << " reqError='" << error
                             << "' errMsg='" << errorMessage
                             << "'";

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchAlbumAndArtist()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Meu Lugar",SearchController::SearchFilterType::Album});
    filters.append({"Anitta",SearchController::SearchFilterType::Artist});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    int status = arguments.at(1).toInt();
    QString error = arguments.at(2).value<QString>();
    QString errorMessage = arguments.at(3).value<QString>();

    QString printError = "Falha na requisição:";
    QTextStream(&printError) << " httpStatus=" << status
                             << " reqError='" << error
                             << "' errMsg='" << errorMessage
                             << "'";

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchAll()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Não Para",SearchController::SearchFilterType::Name});
    filters.append({"Meu Lugar",SearchController::SearchFilterType::Album});
    filters.append({"Anitta",SearchController::SearchFilterType::Artist});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    if(result.getTotal() == 1)
    {
        qDebug() << result;
    }

    int status = arguments.at(1).toInt();
    QString error = arguments.at(2).value<QString>();
    QString errorMessage = arguments.at(3).value<QString>();

    QString printError = "Falha na requisição:";
    QTextStream(&printError) << " httpStatus=" << status
                             << " reqError='" << error
                             << "' errMsg='" << errorMessage
                             << "'";

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchPartAndArtist()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Amor",SearchController::SearchFilterType::Any});
    filters.append({"Anitta",SearchController::SearchFilterType::Artist});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    int status = arguments.at(1).toInt();
    QString error = arguments.at(2).value<QString>();
    QString errorMessage = arguments.at(3).value<QString>();

    QString printError = "Falha na requisição:";
    QTextStream(&printError) << " httpStatus=" << status
                             << " reqError='" << error
                             << "' errMsg='" << errorMessage
                             << "'";

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchOnProgress()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({"Teste",SearchController::SearchFilterType::Any});

    searchCtl.search(filters);

    // Esta segunda chamada deve retornar um exceção devido ao semáfaro
    QVERIFY_EXCEPTION_THROWN(searchCtl.search(filters),Exception);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    int status = arguments.at(1).toInt();
    QString error = arguments.at(2).value<QString>();
    QString errorMessage = arguments.at(3).value<QString>();

    QString printError = "Falha na requisição:";
    QTextStream(&printError) << " httpStatus=" << status
                             << " reqError='" << error
                             << "' errMsg='" << errorMessage
                             << "'";

    QVERIFY2(error.isEmpty(), printError.toStdString().c_str());
}

void SearchControllerTest::testSearchEmptyFilter()
{
    SearchController searchCtl(*this->spotifyWrapper);
    QSignalSpy spy(&searchCtl, &SearchController::searchEnd);

    QVector<SearchController::SearchFilter> filters;

    filters.append({" ",SearchController::SearchFilterType::Any});

    searchCtl.search(filters);

    QVERIFY2(spy.isValid() && spy.wait(), "Fail on search!");

    auto arguments = spy.takeFirst();

    auto result = arguments.at(0).value<PagingForTrack>();
    qDebug() << "Total tracks found: "<< result.getTotal();

    int status = arguments.at(1).toInt();
    QString error = arguments.at(2).value<QString>();
    QString errorMessage = arguments.at(3).value<QString>();

    QString printError = "Falha na requisição:";
    QTextStream(&printError) << " httpStatus=" << status
                             << " reqError='" << error
                             << "' errMsg='" << errorMessage
                             << "'";

    QVERIFY2(error.isEmpty() && result.getTotal() == 0, printError.toStdString().c_str());
}

QTEST_MAIN(SearchControllerTest);
#include "searchcontrollertest.moc"
