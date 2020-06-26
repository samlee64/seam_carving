module Main.Subscriptions exposing (subscriptions)

import Bootstrap.Navbar as Navbar
import Main.Model exposing (Model, Page(..))
import Main.Msg exposing (Msg(..))
import Page.Index as Index
import Page.SeamCarving.Subscriptions as SeamCarving
import Page.Technologies as Technologies


subscriptions : Model -> Sub Msg
subscriptions model =
    let
        pageSub =
            case model.page of
                IndexPage imodel ->
                    Index.subscriptions imodel |> Sub.map IndexMsg

                SeamCarvingPage scmodel ->
                    SeamCarving.subscriptions scmodel |> Sub.map SeamCarvingMsg

                TechnologiesPage imodel ->
                    Technologies.subscriptions imodel |> Sub.map TechnologiesMsg

                NotFound ->
                    Sub.none

        navbarSub =
            Navbar.subscriptions model.navbarState NavbarMsg
    in
    Sub.batch [ pageSub, navbarSub ]
