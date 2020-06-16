module Page.SeamCarving.Subscriptions exposing (subscriptions)

import Bootstrap.Accordion as Accordion
import Bootstrap.Dropdown as Dropdown
import Bootstrap.Tab as Tab
import Page.SeamCarving.Model exposing (Model)
import Page.SeamCarving.Msg exposing (..)
import Time


pollInterval : Float
pollInterval =
    1000


subscriptions : Model -> Sub Msg
subscriptions model =
    Sub.batch
        [ Dropdown.subscriptions model.growForm.numStepsDropdown (GrowFormMsg << NumStepsDropdown)
        , Dropdown.subscriptions model.contentAmplificationForm.factorDropdown (ContentAmplificationFormMsg << FactorDropdown)
        , Accordion.subscriptions model.removeObjectForm.showTriangleData (RemoveObjectFormMsg << AccordionMsg)
        , Time.every pollInterval Tick
        , Tab.subscriptions model.tabState TabMsg
        ]
