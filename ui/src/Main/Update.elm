module Main.Update exposing (update)

import Browser
import Browser.Navigation as Navigation
import Main.Model exposing (Model, Page(..), initPage, updatePage)
import Main.Msg exposing (Msg(..))
import Page.Index as Index
import Page.SeamCarving.Update as SeamCarving
import Url


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case ( msg, model.page ) of
        ( UrlRequest urlRequest, _ ) ->
            case urlRequest of
                Browser.Internal url ->
                    ( model
                    , Navigation.pushUrl model.key (Url.toString url)
                    )

                Browser.External href ->
                    ( model
                    , Navigation.load href
                    )

        ( UrlChange url, _ ) ->
            initPage url model

        ( IndexMsg imsg, IndexPage imodel ) ->
            Index.update imsg imodel |> updatePage IndexPage IndexMsg model

        ( SeamCarvingMsg scmsg, SeamCarvingPage scmodel ) ->
            SeamCarving.update scmsg scmodel |> updatePage SeamCarvingPage SeamCarvingMsg model

        ( NavbarMsg state, _ ) ->
            ( { model | navbarState = state }, Cmd.none )

        _ ->
            ( model, Cmd.none )
