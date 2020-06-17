module Main.View exposing (..)

import Bootstrap.Button as Button
import Bootstrap.Form.Input as Input
import Bootstrap.Grid as Grid
import Bootstrap.Navbar as Navbar
import Bootstrap.Utilities.Spacing as Spacing
import Browser exposing (Document)
import Html exposing (..)
import Html.Attributes exposing (class, href, placeholder, src, style)
import Main.Model exposing (Model, Page(..))
import Main.Msg exposing (Msg(..))
import Page.Index as Index
import Page.SeamCarving.View as SeamCarving


view : Model -> Document Msg
view model =
    { title = ""
    , body = [ viewNavbar model, view_ model ]
    }


view_ : Model -> Html Msg
view_ model =
    div
        [ style "position" "aboslute"
        , style "height" "100vh"
        , style "width" "100vw"
        , Spacing.p5
        ]
        [ viewPage model ]


viewPage : Model -> Html Msg
viewPage model =
    case model.page of
        IndexPage imodel ->
            Html.map IndexMsg <| Index.view imodel

        SeamCarvingPage scmodel ->
            Html.map SeamCarvingMsg <| SeamCarving.view scmodel

        NotFound ->
            div [] [ text "Not Found" ]


viewNavbar : Model -> Html Msg
viewNavbar model =
    Navbar.config NavbarMsg
        |> Navbar.withAnimation
        |> Navbar.collapseMedium
        |> Navbar.info
        |> Navbar.brand
            [ href "/" ]
            [ text " C++ Projects"
            ]
        |> Navbar.items
            [ Navbar.itemLink
                [ href "/sc" ]
                [ text "Seam Carving" ]
            ]
        |> Navbar.customItems
            [ Navbar.customItem <|
                img
                    [ src "assets/images/github.svg"
                    , class "d-inline-block align-top"
                    , style "width" "30px"
                    , Spacing.mr2
                    ]
                    []
            , Navbar.customItem <|
                img [ src "assets/images/linkedin.svg" ] []
            ]
        |> Navbar.view model.navbarState
