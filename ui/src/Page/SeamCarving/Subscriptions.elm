module Page.SeamCarving.Subscriptions exposing (subscriptions)

import Bootstrap.Dropdown as Dropdown
import Bootstrap.Tab as Tab
import Page.SeamCarving.Model exposing (Model)
import Page.SeamCarving.Msg exposing (..)
import Time


pollInterval : Float
pollInterval =
    10000


subscriptions : Model -> Sub Msg
subscriptions model =
    Sub.batch
        [ Dropdown.subscriptions model.growForm.numStepsDropdown (\s -> GrowFormMsg <| NumStepsDropdown s)
        , Dropdown.subscriptions model.contentAmplificationForm.factorDropdown (\s -> ContentAmplificationFormMsg <| FactorDropdown s)
        , Time.every pollInterval Tick
        , Tab.subscriptions model.tabState TabMsg
        ]
