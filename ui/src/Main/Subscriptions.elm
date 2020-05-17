module Main.Subscriptions exposing (subscriptions)

import Main.Model exposing (Model, Page(..))
import Main.Msg exposing (Msg(..))
import Page.Index as Index
import Page.SeamCarving.Subscriptions as SeamCarving


subscriptions : Model -> Sub Msg
subscriptions model =
    case model.page of
        IndexPage imodel ->
            Index.subscriptions imodel |> Sub.map IndexMsg

        SeamCarvingPage scmodel ->
            SeamCarving.subscriptions scmodel |> Sub.map SeamCarvingMsg

        NotFound ->
            Sub.none
